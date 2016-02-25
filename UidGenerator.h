#pragma once
#include <deque>
#include <assert.h>


template<class T, int size = 0>
class UidGenerator
{
private:
	std::deque<T> uid_queue_;


public:
	size_t getSize(void) const
	{
		return uid_queue_.size();
	}

	UidGenerator(void)
	{
		for(T i = 1;i!=size;++i)
		{
			uid_queue_.push_back(i);
		}
	}

	~UidGenerator(void)
	{

	}

	T getUid(void)
	{
		//LOG_IF(FATAL, uid_queue_.empty()==true) << __FUNCTION__ << "() error ";

		T uid = uid_queue_.front();
		uid_queue_.pop_front();
		return uid;
	}

	void freeUid(T uid)
	{
		uid_queue_.push_back(uid);
	}




};

