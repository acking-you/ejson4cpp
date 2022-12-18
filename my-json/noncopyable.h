#ifndef NON_COPYABLE
#define NON_COPYABLE

class noncopyable
{
 public:
	noncopyable() = default;
	noncopyable(noncopyable const&) = delete;
	noncopyable& operator=(noncopyable const&) = delete;
	noncopyable(noncopyable&&) = default;
	noncopyable& operator=(noncopyable&&) = default;
};

#endif