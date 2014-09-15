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
#include <tr1/memory>
#include <vector>
#include <iostream>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm/replace_copy_if.hpp>
#include <sstream>

#include "indentbuf.h"

namespace lccc {

namespace tr1 = std::tr1;

class src {
public:
	typedef tr1::shared_ptr<src> ptr_t;

	virtual ~src() {}

	virtual std::ostream & print(std::ostream & os) const = 0;

protected:
	src()
	{ }

	std::ostream & print_content(std::ostream & os) const
	{
		BOOST_FOREACH(src::ptr_t const& src, content_) {
			src->print(os);
		}

		return os;
	}

	std::vector<src::ptr_t> content_;
};

class comment : public src {
public:
};

class line_comment : public comment {
};

class block_comment : public comment {
};

class cc_src : public src {
public:
	typedef tr1::shared_ptr<cc_src> ptr_t;

protected:
	cc_src()
	{ }
};

class cc_block : public cc_src {
public:
	typedef tr1::shared_ptr<cc_block> ptr_t;

	static ptr_t make()
	{
		return ptr_t(new cc_block());
	}

	std::ostream & print(std::ostream & os) const
	{
		os << "{\n";
		{
			indentbuf indent(os);
			os << source_.str();
		}
		os << "}\n";
		return os;
	}

	std::ostream & src()
	{
		return source_;
	}
private:
	cc_block()
	{ }

	std::stringstream source_;
};

class cpp_src : public src {
public:
	typedef tr1::shared_ptr<cpp_src> ptr_t;
};


class cpp_define : public cpp_src {
public:
	typedef tr1::shared_ptr<cpp_define> ptr_t;

	cpp_define(std::string const& symbol)
	:
		symbol_(symbol)
	{ }

	static ptr_t make(std::string const& symbol)
	{
		return ptr_t(new cpp_define(symbol));
	}

	std::ostream & print(std::ostream & os) const
	{
		return os << "#define " << symbol_ << "\n";
	}

	std::string symbol_;
};

class cpp_include : public cpp_src {
public:
	typedef tr1::shared_ptr<cpp_include> ptr_t;

	cpp_include()
	{
	}

	static ptr_t make()
	{
		return ptr_t(new cpp_include());
	}

	std::ostream & print(std::ostream & os) const
	{
		return os << "#include<foo.h>\n";
	}
};

class cpp_endif : public cpp_src {
public:
	typedef tr1::shared_ptr<cpp_endif> ptr_t;
};

class cpp_ifdef : public cpp_src {
public:
	typedef tr1::shared_ptr<cpp_ifdef> ptr_t;

	cpp_ifdef(std::string const& symbol)
	:
		symbol_(symbol)
	{ }

	static ptr_t make(std::string const& symbol)
	{
		return ptr_t(new cpp_ifdef(symbol));
	}

	void add(src::ptr_t const&)
	{
	}

	std::ostream & print(std::ostream & os) const
	{
		return os << "#ifdef " << symbol_ << "\n";
	}

	std::string symbol_;
};

class cpp_ifndef : public cpp_src {
public:
	typedef tr1::shared_ptr<cpp_ifndef> ptr_t;

	cpp_ifndef(std::string const& symbol)
	:
		symbol_(symbol)
	{ }

	static ptr_t make(std::string const& symbol)
	{
		return ptr_t(new cpp_ifndef(symbol));
	}

	void add(src::ptr_t const& src)
	{
		content_.push_back(src);
	}

	std::ostream & print(std::ostream & os) const
	{
		os << "#ifndef " << symbol_ << "\n";
		print_content(os);
		os << "#endif\n";
		return os;
	}

private:
	std::string symbol_;
};

namespace {

std::string path2guard(std::string const& path)
{
	std::string res;
	boost::replace_copy_if(path, std::back_inserter(res),
		boost::is_any_of(".-/"), '_');
	boost::to_upper(res);
	return res;
}

}

class cpp_guard : public cpp_src {
public:
	typedef tr1::shared_ptr<cpp_guard> ptr_t;

	cpp_guard(std::string const& name)
	:
		ifndef_(cpp_ifndef::make(name))
	{
		ifndef_->add(cpp_define::make(name));
	}

	static ptr_t make(std::string const& name)
	{
		return ptr_t(new cpp_guard(name));
	}

	void add(src::ptr_t const& src)
	{
		ifndef_->add(src);
	}

	std::ostream & print(std::ostream & os) const
	{
		return ifndef_->print(os);
	}

private:
	cpp_ifndef::ptr_t ifndef_;
};

class cc_method_base : public cc_src {
public:
	typedef tr1::shared_ptr<cc_method_base> ptr_t;

	void add_arg(std::string const& type, std::string const& name = "")
	{
		args_.push_back(argument(type, name));
	}

	void define(cc_block::ptr_t const& src)
	{
		src_ = src;
	}

protected:
	struct argument {
		argument(std::string const& type_, std::string const& name_)
		:
			type(type_),
			name(name_)
		{ }

		std::string type;
		std::string name;
	};

	cc_method_base(std::string const& name)
	:
		name_(name)
	{ }

	std::string args() const
	{
		std::string sep;
		std::stringstream res;
		BOOST_FOREACH(argument const& arg, args_) {
			res << sep << arg.type;
			sep = ", ";
		}

		return res.str();
	}

	std::string named_args() const
	{
		std::string sep;
		std::stringstream res;
		BOOST_FOREACH(argument const& arg, args_) {
			res << sep << arg.type;
			if (!arg.name.empty()) {
				res << " " << arg.name;
			}
			sep = ", ";
		}

		return res.str();
	}

	std::string name_;
	std::vector<argument> args_;
	cc_block::ptr_t src_;
};

class cc_method : public cc_method_base {
public:
	typedef tr1::shared_ptr<cc_method> ptr_t;

	static ptr_t make(std::string const& rtype, std::string const& name)
	{
		return ptr_t(new cc_method(rtype, name));
	}

	void make_virtual()
	{
		virtual_ = true;
	}

	void make_abstract()
	{
		virtual_ = true;
		abstract_ = true;
	}

	std::ostream & print(std::ostream & os) const
	{
		os << (virtual_ ? "virtual " : "");
		os << rtype_ << " ";
		os << name_;
		os << "(" << args() << ")";
		if (abstract_) {
			os << " = 0;";
		} else if (src_) {
			os << "\n";
			src_->print(os);
		} else {
			os << ";";
		}
		os << "\n";
		return os;
	}

private:
	cc_method(std::string const& rtype, std::string const& name)
	:
		cc_method_base(name),
		rtype_(rtype),
		virtual_(false),
		abstract_(false)
	{ }

	std::string rtype_;
	bool virtual_;
	bool abstract_;
};

class cc_namespace : public cc_src {
public:
	typedef tr1::shared_ptr<cc_namespace> ptr_t;

	static ptr_t make()
	{
		return ptr_t(new cc_namespace());
	}

	void add(src::ptr_t const& src)
	{
		content_.push_back(src);
	}

	std::ostream & print(std::ostream & os) const
	{
		os << "namespace foo {\n";
		print_content(os);
		os << "}\n";
		return os;
	}

private:

};

class cc_member : public cc_src {
public:
	typedef tr1::shared_ptr<cc_member> ptr_t;
};


class cc_class : public cc_src {
public:
	typedef tr1::shared_ptr<cc_class> ptr_t;

	class constructor : public cc_method_base {
	public:
		typedef tr1::shared_ptr<constructor> ptr_t;

		std::ostream & print(std::ostream & os) const
		{
			os << name_;
			os << "(" << args() << ");\n";
			return os;
		}

	private:
		friend class cc_class;

		constructor(std::string const& name)
		:
			cc_method_base(name)
		{ }
	};

	class destructor : public cc_method_base {
	public:
		typedef tr1::shared_ptr<destructor> ptr_t;

		std::ostream & print(std::ostream & os) const
		{
			os << (virtual_ ? "virtual " : "");
			os << "~" << name_ << "();\n";
			return os;
		}

		void make_virtual()
		{
			virtual_ = true;
		}

	private:
		friend class cc_class;

		destructor(std::string const& name)
		:
			cc_method_base(name),
			virtual_(false)
		{ }

		bool virtual_;
	};

	class visibility : public cc_src {
	public:
		typedef tr1::shared_ptr<visibility> ptr_t;

		std::ostream & print(std::ostream & os) const
		{
			if (!content_.empty()) {
				os << keyword_ << ":\n";
				indentbuf indent(os);
				print_content(os);
			}
			return os;
		}

		void add(cc_method::ptr_t const& src)
		{
			content_.push_back(src);
		}

		void add(cc_member::ptr_t const& src)
		{
			content_.push_back(src);
		}

		void add(constructor::ptr_t const& src)
		{
			content_.push_back(src);
		}

		void add(destructor::ptr_t const& src)
		{
			content_.push_back(src);
		}

	private:
		friend class cc_class;

		visibility(std::string const& keyword)
		:
			keyword_(keyword)
		{ }

		std::string keyword_;
	};


	static ptr_t make(std::string const& name)
	{
		return ptr_t(new cc_class(name));
	}

	constructor::ptr_t make_constructor()
	{
		return constructor::ptr_t(new constructor(name_));
	}

	destructor::ptr_t make_destructor()
	{
		return destructor::ptr_t(new destructor(name_));
	}

	visibility::ptr_t vprivate() const
	{
		return private_;
	}

	visibility::ptr_t vpublic() const
	{
		return public_;
	}

	visibility::ptr_t vprotected() const
	{
		return protected_;
	}

	std::ostream & print(std::ostream & os) const
	{
		os << "class " << name_ << " {\n";
		print_content(os);
		os << "};\n";
		return os;
	}

private:
	cc_class(std::string const& name)
	:
		name_(name),
		public_(new visibility("public")),
		protected_(new visibility("protected")),
		private_(new visibility("private"))
	{
		content_.push_back(public_);
		content_.push_back(protected_);
		content_.push_back(private_);
	}

	std::string name_;
	visibility::ptr_t public_;
	visibility::ptr_t protected_;
	visibility::ptr_t private_;
};

class cc_header : public cc_src {
public:
	typedef tr1::shared_ptr<cc_header> ptr_t;

	cc_header(std::string const& name)
	:
		guard_(cpp_guard::make(path2guard(name)))
	{ }

	static ptr_t make(std::string const& name)
	{
		return ptr_t(new cc_header(name));
	}

	void add(src::ptr_t const& src)
	{
		guard_->add(src);
	}

	std::ostream & print(std::ostream & os) const
	{
		return guard_->print(os);
	}

private:
	cpp_guard::ptr_t guard_;
};

}

int main()
{
	lccc::cc_header::ptr_t header(lccc::cc_header::make("foo.h"));

	lccc::cc_namespace::ptr_t ns1(lccc::cc_namespace::make());
	header->add(ns1);

	lccc::cc_namespace::ptr_t ns2(lccc::cc_namespace::make());
	ns1->add(ns2);

	lccc::cc_class::ptr_t c1(lccc::cc_class::make("Foo"));
	ns2->add(c1);

	lccc::cc_class::constructor::ptr_t cc1(c1->make_constructor());
	c1->vpublic()->add(cc1);

	lccc::cc_method::ptr_t m1(lccc::cc_method::make("void", "foobar"));
	m1->make_abstract();
	m1->add_arg("int", "bla");
	m1->add_arg("std::string const&", "blub");
	c1->vpublic()->add(m1);

	lccc::cc_method::ptr_t m2(lccc::cc_method::make("void", "foobar"));
	m2->add_arg("int", "bla");
	m2->add_arg("std::string const&", "blub");
	lccc::cc_block::ptr_t body(lccc::cc_block::make());

	body->src() << "if (bla) {\n";
	{
		indentbuf indent(body->src());
		body->src() << "do_foo();\n";
	}
	body->src() << "}\n";

	m2->define(body);
	c1->vprivate()->add(m2);

	header->print(std::cout);
}
