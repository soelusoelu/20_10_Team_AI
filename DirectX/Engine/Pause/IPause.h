#pragma once

class IPause {
public:
    virtual ~IPause() = default;
    virtual bool isPausing() const = 0;
};
