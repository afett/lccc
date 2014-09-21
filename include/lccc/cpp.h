/*
   Copyright (c) 2014, Andreas Fett
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright notice, this
     list of conditions and the following disclaimer.

   * Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
   FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
   DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
   SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
   CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
   OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef LCCC_CPP_H
#define LCCC_CPP_H

#include <lccc/base.h>

namespace lccc {

class cpp_define : public src {
public:
	typedef boost::shared_ptr<cpp_define> ptr_t;

	static ptr_t make(std::string const&);
	std::ostream & print(std::ostream &) const;

private:
	cpp_define(std::string const&);

	std::string symbol_;
};

class cpp_include : public src {
public:
	typedef boost::shared_ptr<cpp_include> ptr_t;

	static ptr_t make(std::string const&);
	std::ostream & print(std::ostream &) const;

private:
	cpp_include(std::string const&);

	std::string name_;
};

class cpp_condition : public container {
public:
	typedef boost::shared_ptr<cpp_condition> ptr_t;

	void add(src::ptr_t const&);

	std::ostream & print(std::ostream &) const;

protected:
	cpp_condition(std::string const&, std::string const&);

	std::string symbol_;
	std::string cond_;
};

class cpp_ifdef : public cpp_condition {
public:
	typedef boost::shared_ptr<cpp_ifdef> ptr_t;

	static ptr_t make(std::string const&);
private:
	cpp_ifdef(std::string const&);
};

class cpp_ifndef : public cpp_condition {
public:
	typedef boost::shared_ptr<cpp_ifndef> ptr_t;

	static ptr_t make(std::string const&);
private:
	cpp_ifndef(std::string const&);
};

class cpp_guard : public src {
public:
	typedef boost::shared_ptr<cpp_guard> ptr_t;

	static ptr_t make(std::string const&);
	void add(src::ptr_t const&);
	std::ostream & print(std::ostream & os) const;

private:
	cpp_guard(std::string const&);

	cpp_ifndef::ptr_t ifndef_;
};

}

#endif
