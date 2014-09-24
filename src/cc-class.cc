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
#include <lccc/cc.h>
#include <lccc/indent.h>

namespace lccc {

cc_class::constructor::constructor(std::string const& name)
:
	cc_method_base(name)
{ }

void cc_class::constructor::add(cc_base_class::initializer::ptr_t const& init)
{
	initializers_.push_back(init);
}

std::ostream & cc_class::constructor::print(std::ostream & os) const
{
	os << name_;
	os << "(" << args() << ");\n";
	return os;
}

cc_class::destructor::destructor(std::string const& name)
:
	cc_method_base(name),
	virtual_(false)
{ }

std::ostream & cc_class::destructor::print(std::ostream & os) const
{
	os << (virtual_ ? "virtual " : "");
	os << "~" << name_ << "();\n";
	return os;
}

void cc_class::destructor::make_virtual()
{
	virtual_ = true;
}

cc_class::visibility::visibility(std::string const& keyword)
:
	keyword_(keyword)
{ }

std::ostream & cc_class::visibility::print(std::ostream & os) const
{
	if (!content_.empty()) {
		os << keyword_ << ":\n";
		indent ind(os);
		print_content(os);
	}
	return os;
}

void cc_class::visibility::add(cc_method::ptr_t const& src)
{
	content_.push_back(src);
}

void cc_class::visibility::add(cc_member::ptr_t const& src)
{
	content_.push_back(src);
}

void cc_class::visibility::add(constructor::ptr_t const& src)
{
	content_.push_back(src);
}

void cc_class::visibility::add(destructor::ptr_t const& src)
{
	content_.push_back(src);
}

cc_class::ptr_t cc_class::make(std::string const& name)
{
	return ptr_t(new cc_class(name));
}

cc_class::cc_class(std::string const& name)
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

cc_class::constructor::ptr_t cc_class::make_constructor() const
{
	return constructor::ptr_t(new constructor(name_));
}

cc_class::destructor::ptr_t cc_class::make_destructor() const
{
	return destructor::ptr_t(new destructor(name_));
}

cc_class::visibility::ptr_t cc_class::vprivate() const
{
	return private_;
}

cc_class::visibility::ptr_t cc_class::vpublic() const
{
	return public_;
}

cc_class::visibility::ptr_t cc_class::vprotected() const
{
	return protected_;
}

std::ostream & cc_class::print(std::ostream & os) const
{
	os << "class " << name_ << " {\n";
	print_content(os);
	os << "};\n";
	return os;
}

}
