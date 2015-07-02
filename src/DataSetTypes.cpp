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

#include "pbbam/DataSetTypes.h"
#include "pbbam/internal/DataSetBaseTypes.h"
#include "DataSetUtils.h"
#include <set>
using namespace PacBio;
using namespace PacBio::BAM;
using namespace PacBio::BAM::internal;
using namespace std;

AlignmentSet::AlignmentSet(void)
    : DataSetBase("AlignmentSet")
{ }

BarcodeSet::BarcodeSet(void)
    : DataSetBase("BarcodeSet")
{ }

ConsensusAlignmentSet::ConsensusAlignmentSet(void)
    : DataSetBase("ConsensusAlignmentSet")
{ }

ConsensusReadSet::ConsensusReadSet(void)
    : DataSetBase("ConsensusReadSet")
{ }

ContigSet::ContigSet(void)
    : DataSetBase("ContigSet")
{ }

DataSetBase::DataSetBase(void)
    : BaseEntityType("DataSet")
{ }

DataSetBase::DataSetBase(const string &label)
    : BaseEntityType(label)
{ }

DEFINE_ACCESSORS(DataSetBase, ExternalResources, ExternalResources)

DataSetBase& DataSetBase::ExternalResources(const PacBio::BAM::ExternalResources& resources)
{ ExternalResources() = resources; return *this;  }

DEFINE_ACCESSORS(DataSetBase, Filters, Filters)

DataSetBase& DataSetBase::Filters(const PacBio::BAM::Filters& filters)
{ Filters() = filters; return *this;  }

DEFINE_ACCESSORS(DataSetBase, DataSetMetadata, Metadata)

DataSetBase& DataSetBase::Metadata(const PacBio::BAM::DataSetMetadata& metadata)
{ Metadata() = metadata; return *this; }

const PacBio::BAM::SubDataSets& DataSetBase::SubDataSets(void) const
{
    try {
        return Child<PacBio::BAM::SubDataSets>("DataSets");
    } catch (std::exception&) {
        return internal::NullObject<PacBio::BAM::SubDataSets>();
    }
}

PacBio::BAM::SubDataSets& DataSetBase::SubDataSets(void)
{
    if (!HasChild("DataSets"))
        AddChild(internal::NullObject<PacBio::BAM::SubDataSets>());
    return Child<PacBio::BAM::SubDataSets>("DataSets");
}

DataSetBase& DataSetBase::SubDataSets(const PacBio::BAM::SubDataSets &subdatasets)
{ SubDataSets() = subdatasets; return *this;  }

DataSetBase* DataSetBase::DeepCopy(void) const
{
    DataSetElement* copyDataset = new DataSetElement(*this);
    return static_cast<DataSetBase*>(copyDataset);
}

DataSetBase& DataSetBase::operator+=(const DataSetBase& other)
{
    // must be same dataset types (or 'other' must be generic)
    if (other.Label() != Label() && other.Label() != "DataSet")
        throw std::runtime_error("cannot merge incompatible dataset types");

    // check filter match
    // check object metadata
    Metadata() += other.Metadata();
    ExternalResources() += other.ExternalResources();
    Filters() += other.Filters();
    SubDataSets() += other;

    // add self to

    return *this;
}

std::shared_ptr<DataSetBase> DataSetBase::Create(const string& typeName)
{
    if (typeName == string("DataSet"))       return make_shared<DataSetBase>();
    if (typeName == string("SubreadSet"))    return make_shared<SubreadSet>();
    if (typeName == string("AlignmentSet"))  return make_shared<AlignmentSet>();
    if (typeName == string("BarcodeSet"))    return make_shared<BarcodeSet>();
    if (typeName == string("ConsensusAlignmentSet")) return make_shared<ConsensusAlignmentSet>();
    if (typeName == string("ConsensusReadSet"))      return make_shared<ConsensusReadSet>();
    if (typeName == string("ContigSet"))     return make_shared<ContigSet>();
    if (typeName == string("HdfSubreadSet")) return make_shared<HdfSubreadSet>();
    if (typeName == string("ReferenceSet"))  return make_shared<ReferenceSet>();

    // unknown typename
    throw std::runtime_error("unsupported dataset type");
}

DataSetMetadata::DataSetMetadata(const std::string& numRecords,
                                 const std::string& totalLength)
    : DataSetElement("DataSetMetadata")
{
    NumRecords(numRecords);
    TotalLength(totalLength);
}

DEFINE_ACCESSORS(DataSetMetadata, Provenance, Provenance)

DataSetMetadata& DataSetMetadata::Provenance(const PacBio::BAM::Provenance& provenance)
{ Provenance() = provenance; return *this; }

DataSetMetadata& DataSetMetadata::operator+=(const DataSetMetadata& other)
{
    NumRecords() = NumRecords() + other.NumRecords();
    TotalLength() = TotalLength() + other.TotalLength();
    // merge add'l
    return *this;
}

ExtensionElement::ExtensionElement(void)
    : DataSetElement("ExtensionElement")
{ }

Extensions::Extensions(void)
    : DataSetListElement<ExtensionElement>("Extensions")
{ }

ExternalResource::ExternalResource(void)
    : IndexedDataType("ExternalResource")
{ }

ExternalResource::ExternalResource(const BamFile &bamFile)
    : IndexedDataType("ExternalResource")
{
    MetaType("SubreadFile.SubreadBamFile");
    ResourceId(bamFile.Filename());
}

ExternalResource::ExternalResource(const string& metatype, const string& filename)
    : IndexedDataType("ExternalResource")
{
    MetaType(metatype);
    ResourceId(filename);
}

BamFile ExternalResource::ToBamFile(void) const
{ return BamFile(ResourceId()); }

ExternalResources::ExternalResources(void)
    : DataSetListElement<ExternalResource>("ExternalResources")
{ }

ExternalResources& ExternalResources::operator+=(const ExternalResources& other)
{
    // only keep unique resource ids

    set<std::string> myResourceIds;
    for (size_t i = 0; i < Size(); ++i) {
        const ExternalResource& resource = this->operator[](i);
        myResourceIds.insert(resource.ResourceId());
    }

    vector<size_t> newResourceIndices;
    const size_t numOtherResourceIds = other.Size();
    for (size_t i = 0; i < numOtherResourceIds; ++i) {
        const string& resourceId = other[i].ResourceId();
        auto found = myResourceIds.find(resourceId);
        if (found == myResourceIds.cend())
            newResourceIndices.push_back(i);
    }

    for (size_t index : newResourceIndices)
        Add(other[index]);

    return *this;
}

void ExternalResources::Add(const ExternalResource& ext)
{ AddChild(ext); }

vector<BamFile> ExternalResources::BamFiles(void) const
{
    vector<BamFile> result;
    const int numResources = Size();
    result.reserve(numResources);
    for( const ExternalResource& ext : *this ) {
        result.push_back(ext.ToBamFile());
    }
    return result;
}

void ExternalResources::Remove(const ExternalResource& ext)
{ RemoveChild(ext); }

FileIndex::FileIndex(void)
    : InputOutputDataType("FileIndex")
{ }

FileIndices::FileIndices(void)
    : DataSetListElement<FileIndex>("FileIndices")
{ }

void FileIndices::Add(const FileIndex& index)
{ AddChild(index); }

void FileIndices::Remove(const FileIndex& index)
{ RemoveChild(index); }

Filter::Filter(void)
    : DataSetElement("Filter")
{ }

DEFINE_ACCESSORS(Filter, Properties, Properties)

Filter& Filter::Properties(const PacBio::BAM::Properties& properties)
{ Properties() = properties; return *this; }

Filters::Filters(void)
    : DataSetListElement<Filter>("Filters")
{ }

Filters& Filters::operator+=(const Filters& other)
{
    for (auto& newFilter : other)
        AddChild(newFilter);
    return *this;
}

void Filters::Add(const Filter& filter)
{ AddChild(filter); }

void Filters::Remove(const Filter& filter)
{ RemoveChild(filter); }

HdfSubreadSet::HdfSubreadSet(void)
    : DataSetBase("HdfSubreadSet")
{ }

ParentTool::ParentTool(void)
    : BaseEntityType("ParentTool")
{ }

Properties::Properties(void)
    : DataSetListElement<Property>("Properties")
{ }

void Properties::Add(const Property &property)
{ AddChild(property); }

void Properties::Remove(const Property& property)
{ RemoveChild(property); }

Property::Property(const std::string& name,
                   const std::string& value,
                   const std::string& op)
    : DataSetElement("Property")
{
    Name(name);
    Value(value);
    Operator(op);
}

Provenance::Provenance(void)
    : DataSetElement("Provenance")
{ }

DEFINE_ACCESSORS(Provenance, ParentTool, ParentTool)

ReferenceSet::ReferenceSet(void)
    : DataSetBase("ReferenceSet")
{ }

SubDataSets::SubDataSets(void)
    : internal::DataSetListElement<DataSetBase>("DataSets")
{ }

SubDataSets& SubDataSets::operator+=(const DataSetBase& other)
{
    AddChild(other);
    return *this;
}

SubDataSets& SubDataSets::operator+=(const SubDataSets& other)
{
    for (auto& newSubDataset : other)
        AddChild(newSubDataset);
    return *this;
}

void SubDataSets::Add(const DataSetBase& subdataset)
{ AddChild(subdataset); }

void SubDataSets::Remove(const DataSetBase& subdataset)
{ RemoveChild(subdataset); }

SubreadSet::SubreadSet(void)
    : DataSetBase("SubreadSet")
{ }