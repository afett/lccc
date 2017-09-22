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
	using ptr_t = std::shared_ptr<cc_block>;

	static ptr_t make();
	std::ostream & print(std::ostream & os) const;
	std::ostream & src();
private:
	cc_block();

	std::stringstream source_;
};

class cc_method_base : public src {
public:
	using ptr_t = std::shared_ptr<cc_method_base>;

	void add_arg(std::string const&, std::string name = "");
	cc_block::ptr_t define(cc_block::ptr_t const& src);
	std::string name() const;

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
	using ptr_t = std::shared_ptr<cc_method>;

	static ptr_t make(std::string const&, std::string const&);

	void make_virtual();
	void make_abstract();
	void make_const();

	std::ostream & print(std::ostream & os) const;

private:
	cc_method(std::string const&, std::string const&);

	std::string rtype_;
	bool virtual_;
	bool abstract_;
	bool const_;
};

class cc_namespace : public container {
public:
	using ptr_t = std::shared_ptr<cc_namespace>;

	static ptr_t make(std::string const&);
	src::ptr_t add(src::ptr_t const&);
	std::ostream & print(std::ostream &) const;

private:
	cc_namespace(std::string const&);

	std::string name_;
};

class cc_member : public src {
public:
	using ptr_t = std::shared_ptr<cc_member>;

	static ptr_t make(std::string const&, std::string const&);
	std::ostream & print(std::ostream &) const;

private:
	cc_member(std::string const&, std::string const&);

	std::string name_;
	std::string type_;
};

class cc_base_class : public src {
public:
        using ptr_t = std::shared_ptr<cc_base_class>;

        class initializer : public src {
        public:
                using ptr_t = std::shared_ptr<initializer>;

                std::ostream & print(std::ostream &) const;
        private:
                friend class cc_base_class;

                initializer(std::string const&, std::string const&);

                std::string name_;
                std::string init_;
        };

        static ptr_t make(std::string const&);
        std::ostream & print(std::ostream &) const;
        initializer::ptr_t make_initializer(std::string const&);

private:
        cc_base_class(std::string const&);

        std::string name_;
};

class cc_class : public container {
public:
	using ptr_t = std::shared_ptr<cc_class>;

	class constructor : public cc_method_base {
	public:
		using ptr_t = std::shared_ptr<constructor>;

		std::ostream & print(std::ostream & os) const;
		cc_base_class::initializer::ptr_t
		add(cc_base_class::initializer::ptr_t const&);
	private:
		friend class cc_class;

		constructor(std::string const&);

		std::vector<cc_base_class::initializer::ptr_t> initializers_;
	};

	class destructor : public cc_method_base {
	public:
		using ptr_t = std::shared_ptr<destructor>;

		std::ostream & print(std::ostream &) const;
		void make_virtual();

	private:
		friend class cc_class;

		destructor(std::string const&);

		bool virtual_;
	};

	class visibility : public container {
	public:
		using ptr_t = std::shared_ptr<visibility>;

		std::ostream & print(std::ostream & os) const;
		cc_method::ptr_t add(cc_method::ptr_t const&);
		cc_member::ptr_t add(cc_member::ptr_t const&);
		constructor::ptr_t add(constructor::ptr_t const&);
		destructor::ptr_t add(destructor::ptr_t const&);

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
	cc_base_class::ptr_t add(cc_base_class::ptr_t const&);
	std::string name() const;

private:
	cc_class(std::string const&);

	std::string name_;
	visibility::ptr_t public_;
	visibility::ptr_t protected_;
	visibility::ptr_t private_;
	std::vector<cc_base_class::ptr_t> base_classes_;
};

}

#endif
