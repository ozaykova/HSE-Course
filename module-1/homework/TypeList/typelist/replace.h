#pragma once

#include "typelist.h"

template<typename TList, typename OldType, typename NewType>
struct Replace {
    using NewTypeList = TypeList<typename TList::Head, typename Replace<typename TList::Tail, OldType, NewType>::NewTypeList>;
};

template<typename Tail, typename OldType, typename NewType>
struct Replace<TypeList<OldType, Tail>, OldType, NewType> {
    using NewTypeList = TypeList<NewType, typename Replace<Tail, OldType, NewType>::NewTypeList>;
};

template<typename OldType, typename NewType>
struct Replace<NullType, OldType, NewType> {
    using NewTypeList = NullType;
};