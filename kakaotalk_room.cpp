#include <regex>
#include <stdexcept>
#include "KakaoTalk.hpp"

using std::string;
using std::regex;  using std::regex_match;
using std::domain_error;

namespace kakaotalk {
  room::room(std::istream& in) {
    static regex const room_info("(.*) (\\d+?)( 님과)? 카카오톡 대화");
    chat c(in);
    std::smatch m_info;

    if (regex_match(c.content(), m_info, room_info)) {
      name = m_info[1];
      std::cout << "방제: " << name << std::endl;
      nmemb = std::stoi(m_info[2]);
      std::cout << "인원: " << nmemb << std::endl;
    } else {
      throw domain_error("Unexpected File Format(in Heading).");
    }

    while (in >> c) {
      if (c.isextra())
        continue;
      else if (c.istalk())
        // TODO: mem_info가 _nchat과 _nbyte를 갖도록 수정
        // members[c.name()]+=c.message().length(); // 글자 수로 기록
        ++members[c.name()];
      else if (c.isout())
        members[c.name()].out();
      else if (c.isin() && !c.ismyself())
        members[c.name()].in();
    }

    for (auto& each : members) {
      auto& m_name = each.first;
      auto& m_info = each.second;
      if (m_info)
        enrolls.push_back(std::make_pair(m_name, m_info));
    }

    typedef std::pair<std::string, kakaotalk::mem_info> pairtype;
    enrolls.sort([](const pairtype& x, const pairtype& y)
                 { return !(x.second < y.second); }); // 내림차순 정렬
  }

  
  room::size_type room::member_total() const
  { return nmemb; }

  const std::string& room::room_name() const
  { return name; }
}
