#include "KakaoTalk.hpp"

namespace kakaotalk {
  mem_info::mem_info() : _exist(false), _nchat(0) { }

  bool mem_info::exist() const
  { return _exist; }

  mem_info::operator bool() const
  { return _exist; }

  void mem_info::in() {
    _exist = true;
  }

  void mem_info::out() {
    _exist = false;
  }

  int mem_info::nchat() const
  { return _nchat; }

  mem_info::operator int() const
  { return _nchat; }

  mem_info& mem_info::operator++() {
    _exist = true;
    ++_nchat;
    return *this;
  }

  mem_info& mem_info::operator+=(int n) {
    _exist = true;
    _nchat+=n;
    return *this;
  }

  bool mem_info::operator<(const mem_info& rhs) const {
    return _nchat < rhs._nchat;
  }
  
}
