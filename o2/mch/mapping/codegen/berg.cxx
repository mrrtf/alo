#include "berg.h"
#include "jsonReader.h"
#include <iostream>
#include <map>
#include <string>
#include <iomanip>
#include <fstream>

using namespace jsonmap::codegen;

void assertValid(InputWrapper& doc)
{
  if (!doc.document().HasMember("bergs")) {
    throw std::invalid_argument("input file has no bergs object");
  }
}
std::map<int, std::string> berg2manu(rapidjson::Value& berg, bool is80pins)
{
  int npins{ is80pins ? 80 : 100 };
  int ipin{ 0 };
  if (berg[ipin]["id"].GetInt() != npins) {
    ipin = 1;
  }
  if (berg[ipin]["id"].GetInt() != npins) {
    throw std::runtime_error("number of pins can only be 80 or 100");
  }

  std::array<int, 64> manu2ds;

  std::map<int, std::string> b2manu;

  for (const auto& b : berg[ipin]["pins"].GetArray()) {
    b2manu[b["id"].GetInt()] = b["manu"].GetString();
  }
  return b2manu;
}

std::map<int, std::string> berg2manu(const char* jsonBerg, bool is80pins)
{
  std::cout << "berg2manu " << jsonBerg << "\n";
  std::ifstream in(jsonBerg);
  if (!in) {
    std::cout << "something is wrong with file " << jsonBerg << "\n";
    return {};
  }
  InputWrapper doc(jsonBerg);

  assertValid(doc);

  auto& berg = doc.document()["bergs"];
  return berg2manu(berg, is80pins);
}

void dump(rapidjson::Value& berg, bool is80pins)
{
  std::cout << "---- BERG " << (is80pins ? "80" : "100") << " pins\n";
  std::map<int, std::string> b2manu = berg2manu(berg, is80pins);

  for (int i = (is80pins ? 80 : 100); i > 0; i -= 2) {
    std::cout << std::setw(2) << i - 1 << " " << std::left << std::setw(6) << b2manu[i - 1] << " " << std::setw(2) << i << " " << std::left << std::setw(6) << b2manu[i] << "\n";
  }
}

void dump(const std::map<int, std::string>& b2e)
{
  for (auto p : b2e) {
    std::cout << p.first << "->" << p.second << "\n";
  }
}

std::array<int, 64> getManu2Ds(const char* bergRun2, const char* bergRun3, bool st345)
{
  bool is80pins = (st345 == false);
  std::cout << "is80pins=" << is80pins << "\n";
  auto b2manu = berg2manu(bergRun2, is80pins);
  dump(b2manu);
  auto b2ds = berg2manu(bergRun3, is80pins);
  dump(b2ds);
  std::array<int, 64> manu2ds;
  if (b2manu.empty() || b2ds.empty()) {
    std::cout << "could not get b2e for both runs\n";
    return {};
  }
  for (auto b : b2manu) {
    auto berg = b.first;
    auto manu = isdigit(b.second[0]) ? std::atoi(b.second.c_str()) : -1;
    auto ds = isdigit(b2ds[berg][0]) ? std::atoi(b2ds[berg].c_str()) : -1;
    std::cout << berg << " -> " << manu << " -> " << ds << "\n";
    if (manu >= 0) {
      manu2ds[manu] = ds;
    }
  }
  return manu2ds;
}
