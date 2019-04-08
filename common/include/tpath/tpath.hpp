#ifndef TPATH_HPP__
#define TPATH_HPP__
//
// Created by snikulov on 12/15/17.
//
// Class to iterate over path elements in tree
//
// TODO: Implement iterator for path elements
//

#include <string>
#include <vector>

class tpath
{
    std::string              loc_;
    std::vector<const char*> ptr_;
    bool                     is_root_;

public:

    /// Constant used as path separator
    static constexpr char SEPARATOR = '/';

    /// Default constructor - create empty path
    tpath()
            : loc_{""}, is_root_{*loc_.begin() == SEPARATOR}
    {}

    /// Constructor from string
    /// Will cut-off trailing separator if present
    tpath(const std::string& path)
            : loc_(path.data(),
                   (path.size() > 1 && SEPARATOR == *path.rbegin() ? path.size()-sizeof(SEPARATOR) : path.size()))
            , is_root_{*loc_.begin() == SEPARATOR}
    {
        if (!is_root_)
        {
            ptr_.push_back(loc_.data());
        }

        for(size_t idx = 0; idx < loc_.size(); ++idx)
        {
            if (loc_[idx] == SEPARATOR)
            {
                ptr_.push_back(loc_.data()+idx+sizeof(SEPARATOR));
            }
        }

        if (ptr_.empty())
        {
            ptr_.push_back(loc_.data());
        }
    }

    /// Constructor from plain C-strings
    tpath(const char* cstr)
            : tpath(std::string(cstr))
    {
    }

    bool empty() const
    {
        return loc_.empty();
    }

    size_t elements_count() const
    {
        if (!empty())
        {
            return ptr_.size();
        }
        return 0;
    }

    const std::string& str() const
    {
        return loc_;
    }

    std::string subpath(size_t leftidx) const
    {
        std::string rval;
        if (leftidx < ptr_.size())
        {
            rval = std::string(leftidx == 0 ? loc_.data() : ptr_[leftidx]);
        }
        return rval;
    }

    std::string subpath(size_t leftidx, size_t rightidx) const
    {
        std::string rval;
        if (leftidx < rightidx && rightidx <= ptr_.size())
        {
            if (rightidx == ptr_.size())
            {
                // till the end...
                rval = std::string(leftidx == 0 ? loc_.data() : ptr_[leftidx]);
            }
            else
            {
                rval = std::string(leftidx == 0 ? loc_.data() : ptr_[leftidx]
                        , ptr_[rightidx]-sizeof(char));
            }
        }
        return rval;
    }

    bool is_root_path() const
    {
        return is_root_;
    }

#if 0
    // for debug
    const std::vector<const char*> ptrs() const
    {
        return ptr_;
    }
#endif

    std::string operator[](size_t idx) const
    {
        if (ptr_.size() > idx)
        {
            if (idx == (ptr_.size() - 1))
            {
                return std::string(ptr_[idx], std::distance(ptr_[idx], &*(loc_.end())));
            }
            else
            {
                return std::string(ptr_[idx], std::distance(ptr_[idx], ptr_[idx+1])-1);
            }
        }

        throw std::out_of_range("index " + std::to_string(idx) + " out of range");
    }

    bool operator==(const tpath& rhs) const
    {
        if (this != &rhs)
        {
            return this->loc_ == rhs.loc_;
        }
        return true;
    }

    bool operator!=(const tpath& rhs) const
    {
        return !(*this == rhs);
    }
};

#endif // TPATH_HPP__
