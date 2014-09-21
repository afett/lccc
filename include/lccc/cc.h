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
#ifndef LCCC_CC_H
#define LCCC_CC_H

#include <lccc/base.h>
#include <sstream>

namespace lccc {

class cc_block : public src {
public:
	typedef boost::shared_ptr<cc_block> ptr_t;

	static ptr_t make();
	std::ostream & print(std::ostream & os) const;
	std::ostream & src();
private:
	cc_block();

	std::stringstream source_;
};

class cc_method_base : public src {
public:
	typedef boost::shared_ptr<cc_method_base> ptr_t;

	void add_arg(std::string const&, std::string name = "");
	void define(cc_block::ptr_t const& src);

protected:
	struct argument {
		argument(std::string const&, std::string const&);

		std::string type;
		std::string name;
	};

	cc_method_base(std::string const&);
	std::string args() const;
	std::string named_args() const;

	std::string name_;
	std::vector<argument> args_;
	cc_block::ptr_t src_;
};

class cc_method : public cc_method_base {
public:
	typedef boost::shared_ptr<cc_method> ptr_t;

	static ptr_t make(std::string const&, std::string const&);

	void make_virtual();
	void make_abstract();

	std::ostream & print(std::ostream & os) const;

private:
	cc_method(std::string const&, std::string const&);

	std::string rtype_;
	bool virtual_;
	bool abstract_;
};

class cc_namespace : public container {
public:
	typedef boost::shared_ptr<cc_namespace> ptr_t;

	static ptr_t make(std::string const&);
	void add(src::ptr_t const&);
	std::ostream & print(std::ostream &) const;

private:
	cc_namespace(std::string const&);

	std::string name_;
};

class cc_member : public src {
public:
	typedef boost::shared_ptr<cc_member> ptr_t;

	static ptr_t make(std::string const&, std::string const&);
	std::ostream & print(std::ostream &) const;

private:
	cc_member(std::string const&, std::string const&);

	std::string name_;
	std::string type_;
};

class cc_class : public container {
public:
	typedef boost::shared_ptr<cc_class> ptr_t;

	class constructor : public cc_method_base {
	public:
		typedef boost::shared_ptr<constructor> ptr_t;

		std::ostream & print(std::ostream & os) const;

	private:
		friend class cc_class;

		constructor(std::string const&);
	};

	class destructor : public cc_method_base {
	public:
		typedef boost::shared_ptr<destructor> ptr_t;

		std::ostream & print(std::ostream &) const;
		void make_virtual();

	private:
		friend class cc_class;

		destructor(std::string const&);

		bool virtual_;
	};

	class visibility : public container {
	public:
		typedef boost::shared_ptr<visibility> ptr_t;

		std::ostream & print(std::ostream & os) const;
		void add(cc_method::ptr_t const&);
		void add(cc_member::ptr_t const&);
		void add(constructor::ptr_t const&);
		void add(destructor::ptr_t const&);

	private:
		friend class cc_class;

		visibility(std::string const&);

		std::string keyword_;
	};


	static ptr_t make(std::string const&);

	constructor::ptr_t make_constructor() const;
	destructor::ptr_t make_destructor() const;
	visibility::ptr_t vprivate() const;
	visibility::ptr_t vpublic() const;
	visibility::ptr_t vprotected() const;
	std::ostream & print(std::ostream &) const;

private:
	cc_class(std::string const&);

	std::string name_;
	visibility::ptr_t public_;
	visibility::ptr_t protected_;
	visibility::ptr_t private_;
};

}

#endif