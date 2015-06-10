// Copyright (c) 2014-2015, Pacific Biosciences of California, Inc.
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted (subject to the limitations in the
// disclaimer below) provided that the following conditions are met:
//
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//
//  * Redistributions in binary form must reproduce the above
//    copyright notice, this list of conditions and the following
//    disclaimer in the documentation and/or other materials provided
//    with the distribution.
//
//  * Neither the name of Pacific Biosciences nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
// NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
// GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY PACIFIC
// BIOSCIENCES AND ITS CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL PACIFIC BIOSCIENCES OR ITS
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
// USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
// OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.

// Author: Derek Barnett

#include "pbbam/dataset/ContigSet.h"
#include <exception>
using namespace PacBio;
using namespace PacBio::BAM;

namespace PacBio {
namespace BAM {
namespace internal {

static
const ContigSetMetadata& NullMetadata(void)
{
    static const ContigSetMetadata m;
    return m;
}

} // namespace internal
} // namespace BAM
} // namespace PacBio

ContigSet::ContigSet(void)
    : DataSetBase(DataSetType::CONTIGSET)
{ }

ContigSet::ContigSet(const DataSetBase &other)
    : DataSetBase(other)
{
    if (other.Type() != DataSetType::CONTIGSET)
        throw std::exception();
}

const ContigSetMetadata& ContigSet::Metadata(void) const
{
    try {
        return Child<ContigSetMetadata>("DataSetMetadata");
    } catch (std::exception&) {
        return internal::NullMetadata();
    }
}

ContigSetMetadata& ContigSet::Metadata(void)
{
    if (!HasChild("DataSetMetadata")) {
        ContigSetMetadata newMetadata;
        AddChild(newMetadata);
    }
    return Child<ContigSetMetadata>("DataSetMetadata");
}