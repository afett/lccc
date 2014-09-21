#include <lccc/indent.h>
#include <ostream>

namespace lccc {

indent::indent(std::streambuf* dest)
:
	dest_(dest),
	line_start_(true),
	owner_(NULL)
{ }

indent::indent(std::ostream& dest, std::string indent)
:
	indent_(indent),
	dest_(dest.rdbuf()),
	line_start_(true),
	owner_(&dest)
{
	owner_->rdbuf(this);
}

indent::~indent()
{
	if (owner_ != NULL) {
		owner_->rdbuf(dest_);
	}
}

int indent::overflow(int ch)
{
	if (line_start_ && ch != '\n') {
		dest_->sputn(indent_.c_str(), indent_.size());
	}
	line_start_ = (ch == '\n');
	return dest_->sputc(ch);
}

}
