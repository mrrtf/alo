
function getMotifName(motif) {
    return "" + motif.id;
}

function NS() {
    return "http://www.w3.org/2000/svg";
}

function SVG(w,h){
    var svg=document.createElementNS(NS(),"svg");
    svg.setAttribute("height",h);
    svg.setAttribute("width",w);
    return svg;
}

function rect(w,h){
    var r = document.createElementNS(NS(),"rect");
    r.width.baseVal.value=w;
    r.height.baseVal.value=h;
    r.setAttribute("height",h);
    r.setAttribute("width",w);
    r.setAttribute("class","pad");
    return r;
}

function svgtext(txt,x,y) {
    var t = document.createElementNS(NS(),"text");
    t.textContent=txt;
    t.setAttribute("x",x);
    t.setAttribute("y",y);
    t.setAttribute("class","pad");
    return t;
}

function getMotifExtent(motif,direction) {
    var max=0;
    for ( var i = 0; i < motif.pads.length; ++i ) {
        var p =  motif.pads[i];
        if ( p[direction] > max )  max = p[direction]
    }
    return max+1
}


function getMotifBoundingBox(motif,padsizex, padsizey, size) {
    size.x = padsizex*getMotifExtent(motif,"ix");
    size.y = padsizey*getMotifExtent(motif,"iy");
}

function getMotifSVGSymbol(motif, padsizex, padsizey) {
    var size = {};
    getMotifBoundingBox(motif,padsizex,padsizey,size);
    var svg = document.createElementNS(NS(),"svg");
    symbol = document.createElementNS(NS(),"symbol");
    symbol.id=getMotifName(motif);
    symbol.setAttribute("viewBox","0 0 " + size.x + " " + size.y);
    svg.appendChild(symbol);
    var pads = motif.pads;
    for ( var i = 0; i < pads.length; ++i ) {
        var p = pads[i]
        var padgroup = document.createElementNS(NS(),"g");
        var r = rect(padsizex,padsizey);
        x = p.ix*padsizex;
        r.x.baseVal.value = x;
        y = size.y-padsizey-p.iy*padsizey;
        r.y.baseVal.value = y;
        padgroup.appendChild(r);
        var t = svgtext(p.berg,x+padsizex/2,y+padsizey/2);
        t.setAttribute("dy",padsizey/5)
        t.setAttribute("dx",-padsizex/5)
        t.setAttribute("font-size",padsizex/3);
        padgroup.appendChild(t);
        symbol.appendChild(padgroup);
    }
    return svg
}

function getMotifSVGRef(motif) {
    svg = document.createElementNS(NS(),"svg");
    svg.setAttribute("class","motif");
    ref = document.createElementNS(NS(),"use");
    ref.setAttribute("href","#" + getMotifName(motif));
    svg.appendChild(ref);
    return svg;
}

function defineSVGMotifs(motifs,padsizex,padsizey) {
    symbols = document.getElementById("svgsymbols");
    for ( x in motifs) {
        svg = getMotifSVGSymbol(motifs[x], padsizex, padsizey);
        symbols.appendChild(svg);
    }
}

function getMotifByName(motifname) {
    return motifs.find(s => { return s.id == motifname; });
}

function plotMotifByName(motifname) {
    console.log("plotMotifByName " + motifname);
   motif = getMotifByName(motifname);
   if (typeof motif != 'undefined') {
       plotMotif(motif);
   } else
   {
       alert("motif " + motifname + " does not exist");
   }
}

function showAllMotifNames(motifs) {
    var motiflist = document.getElementById("motiflist");
    var ul = document.createElement("ul");
    motiflist.appendChild(ul);
    for ( x in motifs) {
        text = getMotifName(motifs[x]);
        var li = document.createElement("li");
        li.setAttribute("id","li"+text);
        li.setAttribute("class","motifid");
        li.innerHTML = text;
        ul.appendChild(li);
    }
    $("li.motifid").click(function() { plotMotifByName($(this).attr('id').replace("li","")); });

}

function decodeJSON(json) {
    const padsizex=63/2;
    const padsizey=42/3;
    motifs=json.motifs;
    defineSVGMotifs(motifs,padsizex,padsizey);
    plotMotifByName("E15");
    showAllMotifNames(motifs);
    //plotAllMotifs(motifs);
}

function plotMotif(motif) {
    var plot = document.getElementById("plot");
    $('#plot').empty();
    var svg = getMotifSVGRef(motif);
    var motifNode = document.createElement("div");
    motifNode.className = "motif";
    var p = document.createElement("p");
    p.innerHTML = getMotifName(motif) + " npads=" + motif.pads.length;
    motifNode.appendChild(p);
    motifNode.appendChild(svg);
    plot.appendChild(motifNode);
}

function plotAllMotifs(motifs) {
    for ( x in motifs) {
        text = getMotifName(motifs[x]);
        if (text.indexOf("1B-") != -1) {
            plotMotif(motifs[x]);
        }
    }
}

$(function() {
    $(".button").click(function() {

        var text = $("input#motifname").val();
        motif = getMotifByName(text);
        if (typeof motif != 'undefined') {
            plotMotif(motif);
        }
        else {
            alert("motif " + text + " does not exist");
        }
    });

    fetch("http://localhost:8080/motif.json",
        { method: 'get',
            headers: {
                'Accept': 'application/json'
            }}
    ).then(response => response.json())
        .then(json => decodeJSON(json));

});
