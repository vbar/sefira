#include "xbuffer.hh"
#include <stdexcept>

XBuffer::XBuffer():
    buffer(xmlBufferCreate())
{
    if (!buffer) {
	throw std::runtime_error("cannot create buffer");
    }
}

XBuffer::~XBuffer()
{
    xmlBufferFree(buffer);
}

