
#ifndef __TBA_BYTE_BUFFER_H__
#define __TBA_BYTE_BUFFER_H__


#include <stdlib.h>
#include <string.h>
#include "tba_types.h"

class tba_byte_buffer
{
public:
    inline tba_byte_buffer()
        : buffer_(NULL)
        , buffer_size_(0)
        , data_begin_index_(0)
        , data_end_index_(0)
        , owner_(true)
    {
    }

    inline tba_byte_buffer(int buffer_size)
        : data_begin_index_(0)
        , data_end_index_(0)
        , owner_(true)
    {
        char *new_buffer = (char *)malloc(buffer_size);
        if (NULL != new_buffer)
        {
            buffer_      = new_buffer;
            buffer_size_ = buffer_size;
        }
        else
        {
            buffer_      = NULL;
            buffer_size_ = 0;
        }
    }

	inline tba_byte_buffer(char *buff, int len)
		:data_begin_index_(0),
		data_end_index_(len),
		owner_(true)
	{
		owner_ = false;
		char *new_buffer = (char*)malloc(len);
		if (NULL != new_buffer)
		{
			memcpy(new_buffer, buff, len);
			buffer_ = new_buffer;
			buffer_size_ = len;
			owner_ = true;
		}
		else
		{
			buffer_      = NULL;
			buffer_size_ = 0;
		}
	}

    inline tba_byte_buffer(const tba_byte_buffer &buf)
    {
        if (buf.owner_)
        {
            buf.owner_          = false;
            buffer_             = buf.buffer_;
            buffer_size_        = buf.buffer_size_;
            data_begin_index_   = buf.data_begin_index_;
            data_end_index_     = buf.data_end_index_;
            owner_              = true;
        }
        else
        {
            buffer_             = NULL;
            buffer_size_        = 0;
            data_begin_index_   = 0;
            data_end_index_     = 0;
            owner_              = false;
        }
    }

    inline ~tba_byte_buffer()
    {
        if (owner_ && (NULL != buffer_))
        {
            free(buffer_);
        }
    }

    inline tba_byte_buffer& operator= (const tba_byte_buffer &buf)
    {
        if (this != &buf)
        {
            if (owner_ && (NULL != buffer_))
            {
                free(buffer_);
            }

            if (buf.owner_)
            {
                buf.owner_          = false;
                buffer_             = buf.buffer_;
                buffer_size_        = buf.buffer_size_;
                data_begin_index_   = buf.data_begin_index_;
                data_end_index_     = buf.data_end_index_;
                owner_              = true;
            }
            else
            {
                buffer_             = NULL;
                buffer_size_        = 0;
                data_begin_index_   = 0;
                data_end_index_     = 0;
                owner_              = false;
            }
        }
        return *this;
    }

    inline void clear()
    {
        if (owner_ && (NULL != buffer_))
        {
            free(buffer_);
        }
        buffer_             = NULL;
        buffer_size_        = 0;
        data_begin_index_   = 0;
        data_end_index_     = 0;
        owner_              = true;
    }

    inline void reset()
    {
        data_begin_index_   = 0;
        data_end_index_     = 0;
    }

    inline char* buffer() const
    {
        return buffer_;
    }

    inline int buffer_size() const
    {
        return buffer_size_;
    }

    inline bool data(const char *data, int size, bool owner = true)
    {
        if (size <= 0)
        {
            return false;
        }
        if (NULL == data)
        {
            return false;
        }
        if ( (data >= buffer_) && (data < (buffer_ + buffer_size_)) )
        {
            return false;
        }

        if (owner)
        {
            char *new_buffer;
            if (owner_)
            {
                new_buffer = (char *)realloc(buffer_, size);
            }
            else
            {
                new_buffer = (char *)malloc(size);
            }
            if (NULL == new_buffer)
            {
                return false;
            }
            memcpy(new_buffer, data, size);
            buffer_ = new_buffer;
        }
        else
        {
            if (owner_ && (buffer_ != NULL))
            {
                free(buffer_);
            }
            buffer_ = (char *)data;
        }
        buffer_size_        = size;
        data_begin_index_   = 0;
        data_end_index_     = size;
        owner_              = owner;
        return true;
    }

    inline char* data() const
    {
        return buffer_ + data_begin_index_;
    }

    inline int data_size() const
    {
        return data_end_index_ - data_begin_index_;
    }

    //返回未使用的容量
    inline int free_size() const
    {
        return buffer_size_ - data_end_index_;
    }

    inline int data_begin() const
    {
        return data_begin_index_;
    }

    void data_begin(int new_begin_index)
    {
        if (new_begin_index > buffer_size_)
        {
            new_begin_index = buffer_size_;
        }
        data_begin_index_ = new_begin_index;
        if (data_begin_index_ > data_end_index_)
        {
            data_end_index_ = data_begin_index_;
        }
    }

    inline int data_end() const
    {
        return data_end_index_;
    }

    inline void data_end(int new_end_index)
    {
        if (new_end_index > buffer_size_)
        {
            new_end_index = buffer_size_;
        }
        data_end_index_ = new_end_index;
        if (data_begin_index_ > data_end_index_)
        {
            data_begin_index_ = data_end_index_;
        }
    }

    inline bool empty() const
    {
        return data_end_index_ - data_begin_index_ == 0;
    }

    inline bool owner() const
    {
        return owner_;
    }

    inline void owner(bool owner)
    {
        owner_ = owner;
    }

    bool reserve(int new_size)
    {
        if (owner_)
        {
            if (new_size > buffer_size_)
            {
                char *new_buffer = (char *)realloc(buffer_, new_size);
                if (NULL != new_buffer)
                {
                    buffer_             = new_buffer;
                    buffer_size_        = new_size;
                    data_begin_index_   = 0;
                    data_end_index_     = 0;
                    return true;
                }
            }
        }
        else
        {
            char *new_buffer = (char *)malloc(new_size);
            if (NULL != new_buffer)
            {
                buffer_             = new_buffer;
                buffer_size_        = new_size;
                data_begin_index_   = 0;
                data_end_index_     = 0;
                owner_              = true;
                return true;
            }
        }
        return false;
    }

    inline bool resize(int new_size)
    {
        return reserve(new_size);
    }

	inline void mzero()
	{
		memset(buffer_, 0x00, buffer_size_);
	}

    inline bool write(const char *data, int size, int multiple = 100)
    {
        if (!owner_)
        {
            return false;
        }

        if (free_size() >= size)
        {
            memcpy(buffer_ + data_end_index_, data, size);
            data_end_index_ += size;
        }
        else
        {
            return adjust_buffer_i(data, size, multiple);
        }
        return true;
    }

    inline bool write(const char *data)
    {
        return write(data, (int)(strlen(data) + 1));
    }

    inline bool write(char c)
    {
        return write((const char *)&c, sizeof(char));
    }

    inline bool write(short n)
    {
        return write((const char *)&n, sizeof(short));
    }

    inline bool write(int n)
    {
        return write((const char *)&n, sizeof(int));
    }

    inline bool write(int64 n)
    {
        return write((const char *)&n, sizeof(int64));
    }

    inline bool write(float f)
    {
        return write((const char *)&f, sizeof(float));
    }

    inline bool write(double d)
    {
        return write((const char *)&d, sizeof(double));
    }

    inline int read(char *data, int size)
    {
        if (data_size() >= size)
        {
            memcpy(data, buffer_ + data_begin_index_, size);
            data_begin_index_ += size;
            return size;
        }
        return -1;
    }

    inline char* read(int size)
    {
        if (data_size() >= size)
        {
            char *ret = (buffer_ + data_begin_index_);
            data_begin_index_ += size;
            return ret;
        }
        return NULL;
    }

private:
    inline bool adjust_buffer_i(const char *data, int size, int multiple)
    {
        int  new_size    = buffer_size_ * multiple / 100 + buffer_size_ + size;
        char *new_buffer = (char *)realloc(buffer_, new_size);
        if (NULL != new_buffer)
        {
            memcpy(new_buffer + data_end_index_, data, size);
            buffer_         = new_buffer;
            buffer_size_    = new_size;
            data_end_index_ += size;
            return true;
        }
        return false;
    }

private:
    char            *buffer_;
    int             buffer_size_;
    int             data_begin_index_;
    int             data_end_index_;
    mutable bool    owner_;
};



#endif

