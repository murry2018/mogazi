#include <fstream>
#include <vector>
#include <algorithm>
#include "KakaoTalk.hpp"
using namespace std;

int main(int argc, char **argv) {
  if (argc != 2) {
    string filename = argv[0];
    string::size_type loc=0, ploc;
    while ((ploc = filename.substr(loc).find('/')) != string::npos)
      loc = ploc+1;
    cout << "usage: " << string(filename.begin() + loc, filename.end())
         << " <Kakaotalk datafile>" << endl;
    return 0;
  }
  ifstream chats(argv[1]);
  kakaotalk::room myroom(chats);
  vector<string> mogazi;

  for (auto& each : myroom.enrolls) {
    auto& m_name = each.first;
    auto& m_info = each.second;

    cout << m_name << ", 한 말의 수: " << int(m_info) << endl;
    if (int(m_info) < 20) {
      mogazi.push_back(m_name);
    }
  }

  sort(mogazi.begin(), mogazi.end());
  cout << "안녕히 가세요 " << endl;
  for (auto& each : mogazi) {
    cout << each << " ";
  }
  cout << endl;

  cout << myroom.member_total() - myroom.enrolls.size()
       << " 명이 조사 결과에서 누락되었습니다." << endl;
  return 0;
}
