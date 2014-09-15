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
#ifndef INDENTBUF_H
#define INDENTBUF_H

// http://stackoverflow.com/a/9600752

#include <iostream>

class indentbuf : public std::streambuf {
public:
	explicit indentbuf(std::streambuf* dest)
	:
		dest_(dest),
		line_start_(true),
		owner_(NULL)
	{ }

	explicit indentbuf(std::ostream& dest)
	:
		dest_(dest.rdbuf()),
		line_start_(true),
		owner_(&dest)
	{
		owner_->rdbuf(this);
	}

	virtual ~indentbuf()
	{
		if (owner_ != NULL) {
			owner_->rdbuf(dest_);
		}
	}

protected:
	virtual int overflow(int ch)
	{
		if (line_start_ && ch != '\n') {
			dest_->sputc('\t');
		}
		line_start_ = (ch == '\n');
		return dest_->sputc(ch);
	}

private:
	std::streambuf* dest_;
	bool line_start_;
	std::ostream* owner_;
};

#endif
