#ifndef __SMART_POINTER_H__
#define __SMART_POINTER_H__

#include "DefaultConstant.h"

class ReferenceCounter
{
public:
	void addReference()
	{
		count_++;
	}

	int release()
	{
		return --count_;
	}

private:
	int count_;
};



template<typename T>
class SmartPointer {
public:
	SmartPointer() : source_(0), referenceCounter_(0)
	{
		referenceCounter_ = new ReferenceCounter();
		referenceCounter_->addReference();
	}



	SmartPointer(T* source) : source_(source), referenceCounter_(0)
	{
		referenceCounter_ = new ReferenceCounter();
		referenceCounter_->addReference();
	}



	SmartPointer(const SmartPointer<T>& source) :
		source_(source.source_),
		referenceCounter_(source.referenceCounter)
	{
		referenceCounter_->addReference();
	}


	~SmartPointer()
	{
		if (referenceCounter_->release())
			SAFE_RELEASE(source_);
			SAFE_RELEASE(referenceCounter_);
	}


	template <class T>
	SmartPointer<T>& operator= (const SmartPointer<T>& sourcePointer)
	{
		if (this != &sourcePointer) {

			if (referenceCounter_->release()) {
				SAFE_RELEASE(source_);
				SAFE_RELEASE(referenceCounter_);
			}

			source_ = sourcePointer.source_;
			referenceCounter_ = referenceCounter_;
			referenceCounter_->addReference();
		}
		return *this;
	}



	T& operator*()
	{
		return *source_;
	}


	T* getSource()
	{
		return source_;
	}



	T* operator->()
	{
		return source_;
	}



	bool isNull() const
	{
		return source_ == 0;
	}



	T* source_;
private:
	ReferenceCounter* referenceCounter_;
};

#endif