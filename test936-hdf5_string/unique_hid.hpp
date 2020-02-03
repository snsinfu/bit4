#pragma once

#include <utility>

#include "hdf5.h"


// RAII wrapper for an hid_t.
template<herr_t(& close_fn)(hid_t)>
class unique_hid
{
public:
    // The constructor takes the ownership of given hid. The hid can be negative
    // (in that case the created `unique_hid` instance does nothing).
    unique_hid(hid_t hid)
        : _hid{hid}
    {
    }

    ~unique_hid() noexcept
    {
        if (_hid >= 0) {
            close_fn(_hid);
        }
    }

    unique_hid(unique_hid const&) = delete;

    unique_hid(unique_hid&& other) noexcept
        : _hid{other._hid}
    {
        other._hid = -1;
    }

    unique_hid& operator=(unique_hid&& other) noexcept
    {
        unique_hid tmp = std::move(other);
        swap(tmp);
        return *this;
    }

    void swap(unique_hid& other) noexcept
    {
        auto tmp = _hid;
        _hid = other._hid;
        other._hid = tmp;
    }

    operator hid_t() const noexcept
    {
        return _hid;
    }

private:
    hid_t _hid;
};
