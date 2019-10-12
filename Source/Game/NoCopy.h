#pragma once

namespace ME
{
class NoCopy
{
protected:
	NoCopy() = default;

private:
	NoCopy(const NoCopy&) = delete;
	NoCopy& operator=(const NoCopy&) = delete;
};
} // namespace ME
