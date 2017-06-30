/**
 * Created by laurent on 30/06/2017.
 */

function getMotifName(motif) {
    return "[" + motif["id"] + "]";
}

function SVG(w,h){
    var NS="http://www.w3.org/2000/svg";
    var svg=document.createElementNS(NS,"svg");
    svg.width=w;
    svg.height=h;
    svg.setAttribute("height",h);
    svg.setAttribute("width",w);
    svg.style.stroke="black";
    return svg;
}

function rect(w,h){
    var NS="http://www.w3.org/2000/svg";
    var r = document.createElementNS(NS,"rect");
    r.width.baseVal.value=w;
    r.height.baseVal.value=h;
    r.setAttribute("height",h);
    r.setAttribute("width",w);
    r.setAttribute("class","pad");
    return r;
}

function svgtext(txt,x,y) {
    var NS="http://www.w3.org/2000/svg";
    var t = document.createElementNS(NS,"text");
    t.textContent=txt;
    t.setAttribute("x",x);
    t.setAttribute("y",y);
    t.setAttribute("class","pad");
    return t;
}

function getMotifExtent(motif,direction) {
    max=0;
    for ( var i = 0; i < motif.pads.length; ++i ) {
        var p =  motif.pads[i];
        if ( p[direction] > max )  max = p[direction]
    }
    return max+1
}

function getMotifSVG(motif,padsizex,padsizey) {
    var mx = getMotifExtent(motif,"ix");
    var my = getMotifExtent(motif,"iy");
    pads = motif.pads;
    console.log("motif " + motif.id + " mx=" + mx + " my=" + my + " npads=" + pads.length);
    var svg = SVG(mx*padsizex,my*padsizey);
    var NS="http://www.w3.org/2000/svg";
    g = document.createElementNS(NS,"g");
    g.style.fill="#DDDDDD";
    svg.appendChild(g);
    n = 0;
    for ( var i = 0; i < pads.length; ++i ) {
        var p = pads[i]
        var padgroup = document.createElementNS(NS,"g");
        var r = rect(padsizex,padsizey);
        x = p.ix*padsizex;
        r.x.baseVal.value = x;
        y = (my-1)*padsizey-p.iy*padsizey;
        r.y.baseVal.value = y;
        padgroup.appendChild(r);
        var t = svgtext(p.berg,x+padsizex/2,y+padsizey/2);
        t.setAttribute("dy",padsizey/5)
        t.setAttribute("dx",-padsizex/5)
        t.setAttribute("font-size",padsizex/3);
        t.setAttribute("font-family","verdana");
        console.log(p.berg);
        padgroup.appendChild(t);
        g.appendChild(padgroup);
        ++n;
    }
    console.log(n)
    return svg
}

function decodeJSON(json) {
    motifs = json.motifs;
    n=0;
    const padsizex=63/2;
    const padsizey=42/3;
    plot = document.getElementById("plot");
    for ( x in motifs) {
        text = getMotifName(motifs[x]);
        if (text.indexOf("-") == -1) {
            svg = getMotifSVG(motifs[x], padsizex, padsizey);
            motifNode = document.createElement("div");
            motifNode.className = "motif";
            var p = document.createElement("p");
            p.innerHTML = text;
            motifNode.appendChild(p);
            motifNode.appendChild(svg);
            plot.appendChild(motifNode);
            ++n;
        }
    }
}
