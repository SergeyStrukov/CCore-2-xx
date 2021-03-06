/* DDLMap.cpp */
//----------------------------------------------------------------------------------------
//
//  Project: CCore 2.00
//
//  Tag: Applied
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2015 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#include <CCore/inc/ddl/DDLMap.h>

#include <CCore/inc/Exception.h>
#include <CCore/inc/algon/CommonIntAlgo.h>

namespace CCore {
namespace DDL {

/* functions */

void GuardMapStructNameDuplication()
 {
  Printf(Exception,"CCore::DDL::Map : struct name duplication");
 }

/* class Map */

template <ulen N>
class Map::AlignMulTable : NoCopy
 {
   ulen table[N];

  public:

   AlignMulTable()
    {
     for(ulen a=0; a<N ;a++) table[a]=N/Algon::GCD(a,N);
    }

   ulen operator () (ulen a) const { return table[a%N]; } // N/GCD(a,N)

   static AlignMulTable<N> Object;
 };

template <ulen N>
Map::AlignMulTable<N> Map::AlignMulTable<N>::Object;

template <>
class Map::AlignMulTable<1>
 {
  public:

   static ulen Object(ulen)
    {
     return 1;
    }
 };

template <>
class Map::AlignMulTable<2>
 {
  public:

   static ulen Object(ulen a)
    {
     return (a&1)?2:1;
    }
 };

template <>
class Map::AlignMulTable<4>
 {
  public:

   static ulen Object(ulen a)
    {
     static const ulen Table[4]={1,4,2,4};

     return Table[a&3];
    }
 };

template <>
class Map::AlignMulTable<8>
 {
  public:

   static ulen Object(ulen a)
    {
     static const ulen Table[8]={1,8,4,8,2,8,4,8};

     return Table[a&7];
    }
 };

template <class T>
ulen Map::AlignAdd(ulen a)
 {
  return a*AlignMulTable<alignof (T)>::Object(a);
 }

ulen Map::AlignOf(unsigned flags)
 {
  ulen ret=1;

  if( flags&(1u<<TypeTag_sint) )    ret=AlignAdd<sint_type>(ret);
  if( flags&(1u<<TypeTag_uint) )    ret=AlignAdd<uint_type>(ret);
  if( flags&(1u<<TypeTag_ulen) )    ret=AlignAdd<ulen_type>(ret);

  if( flags&(1u<<TypeTag_sint8) )   ret=AlignAdd<sint8>(ret);
  if( flags&(1u<<TypeTag_uint8) )   ret=AlignAdd<uint8>(ret);
  if( flags&(1u<<TypeTag_sint16) )  ret=AlignAdd<sint16>(ret);
  if( flags&(1u<<TypeTag_uint16) )  ret=AlignAdd<uint16>(ret);
  if( flags&(1u<<TypeTag_sint32) )  ret=AlignAdd<sint32>(ret);
  if( flags&(1u<<TypeTag_uint32) )  ret=AlignAdd<uint32>(ret);
  if( flags&(1u<<TypeTag_sint64) )  ret=AlignAdd<sint64>(ret);
  if( flags&(1u<<TypeTag_uint64) )  ret=AlignAdd<uint64>(ret);

  if( flags&(1u<<TypeTag_text) )    ret=AlignAdd<StrLen>(ret);
  if( flags&(1u<<TypeTag_ptr) )     ret=AlignAdd<DataPtr>(ret);
  if( flags&(1u<<TypeTag_polyptr) ) ret=AlignAdd<TypedDataPtr>(ret);
  if( flags&(1u<<TypeTag_array) )   ret=AlignAdd<ArrayPtr>(ret);

  return ret;
 }

ulen Map::AlignFor(ulen off,unsigned flags)
 {
  if( !TryAlign(off,AlignOf(flags)) ) GuardMapLenOverflow();

  return off;
 }

struct Map::PrepareFunc
 {
  Map *map;
  Value value;
  RecValue &rec;

  PrepareFunc(Map *map_,const Value &value_,RecValue &rec_) : map(map_),value(value_),rec(rec_) {}

  void operator () (TypeNode::Base *type_ptr)
   {
    if( type_ptr->isText() )
      {
       RecValue *obj=map->rec_buf.append_default();

       rec.elems=Range(obj,1);
      }
   }

  void operator () (TypeNode::Ptr *) {}

  void operator () (TypeNode::PolyPtr *) {}

  void array(TypeNode *type)
   {
    PtrLen<Value> data=value.get<Block>().data;

    PtrLen<RecValue> elems=map->rec_buf.extend_default(data.len);

    rec.elems=elems;

    for(; +data ;++data,++elems) map->prepare(type,*data,*elems);
   }

  void operator () (TypeNode::Array *type_ptr)
   {
    array(type_ptr->type_node);
   }

  void operator () (TypeNode::ArrayLen *type_ptr)
   {
    array(type_ptr->type_node);
   }

  void operator () (StructNode *struct_node)
   {
    PtrLen<Value> data=value.get<Block>().data;

    PtrLen<RecValue> elems=map->rec_buf.extend_default(data.len);

    rec.elems=elems;

    auto cur=begin(struct_node->field_list);

    for(; +data ;++data,++elems,++cur) map->prepare((*cur).type_node,*data,*elems);
   }
 };

void Map::prepare(TypeNode *type,const Value &value,RecValue &rec)
 {
  incLevel();

  TypeAdapter(type).ptr.apply(PrepareFunc(this,value,rec));

  decLevel();
 }

void Map::prepare()
 {
  for(ulen i=0,n=const_buf.getLen(); i<n ;i++)
    {
     auto &result=eval->const_table[i];

     resetLevel();

     prepare(result.type,result.value,const_buf[i]);
    }
 }

auto Map::layout(StructNode *struct_node) -> SizeInfo
 {
  auto &rec=struct_buf[struct_node->index];

  if( rec.struct_node ) return rec;

  ulen off=0;
  unsigned flags=0;

  for(FieldNode &field_node : struct_node->field_list )
    {
     SizeInfo field=sizeOf(field_node.type_node);

     off=AlignFor(off,field.flags);

     field_node.index=off;

     off=MapAddLen(off,field.size_of);

     flags|=field.flags;
    }

  SizeInfo ret(AlignFor(off,flags),flags);

  rec.set(ret,struct_node);

  NameKey key(struct_node);
  StructAlgo::PrepareIns prepare(struct_map,key);

  if( prepare.found )
    {
     GuardMapStructNameDuplication();
    }
  else
    {
     prepare.complete(&rec);
    }

  return ret;
 }

struct Map::SizeOfFunc
 {
  Map *map;
  SizeInfo &ret;

  SizeOfFunc(Map *map_,SizeInfo &ret_) : map(map_),ret(ret_) {}

  void operator () (TypeNode::Base *type_ptr)
   {
    switch( type_ptr->type )
      {
       case TypeNode::Base::Type_sint   : ret.set(sizeof (sint_type),TypeTag_sint); break;
       case TypeNode::Base::Type_uint   : ret.set(sizeof (uint_type),TypeTag_uint); break;
       case TypeNode::Base::Type_ulen   : ret.set(sizeof (ulen_type),TypeTag_ulen); break;

       case TypeNode::Base::Type_sint8  : ret.set(sizeof (sint8),TypeTag_sint8); break;
       case TypeNode::Base::Type_uint8  : ret.set(sizeof (uint8),TypeTag_uint8); break;
       case TypeNode::Base::Type_sint16 : ret.set(sizeof (sint16),TypeTag_sint16); break;
       case TypeNode::Base::Type_uint16 : ret.set(sizeof (uint16),TypeTag_uint16); break;
       case TypeNode::Base::Type_sint32 : ret.set(sizeof (sint32),TypeTag_sint32); break;
       case TypeNode::Base::Type_uint32 : ret.set(sizeof (uint32),TypeTag_uint32); break;
       case TypeNode::Base::Type_sint64 : ret.set(sizeof (sint64),TypeTag_sint64); break;
       case TypeNode::Base::Type_uint64 : ret.set(sizeof (uint64),TypeTag_uint64); break;

       case TypeNode::Base::Type_text   : ret.set(sizeof (StrLen),TypeTag_text); break;
       case TypeNode::Base::Type_ip     : ret.set(sizeof (uint32),TypeTag_uint32); break;
      }
   }

  void operator () (TypeNode::Ptr *)
   {
    ret.set(sizeof (DataPtr),TypeTag_ptr);
   }

  void operator () (TypeNode::PolyPtr *)
   {
    ret.set(sizeof (TypedDataPtr),TypeTag_polyptr);
   }

  void operator () (TypeNode::Array *)
   {
    ret.set(sizeof (ArrayPtr),TypeTag_array);
   }

  void operator () (TypeNode::ArrayLen *type_ptr)
   {
    TypeNode *type=type_ptr->type_node;

    ret=map->sizeOf(type);

    ret.mul( ulen(map->eval->getLen(type_ptr->len_node)) );
   }

  void operator () (StructNode *struct_node)
   {
    ret=map->layout(struct_node);
   }
 };

auto Map::sizeOf(TypeNode *type) -> SizeInfo
 {
  SizeInfo ret;

  incLevel();

  TypeAdapter(type).ptr.apply(SizeOfFunc(this,ret));

  decLevel();

  return ret;
 }

void Map::sizeOf()
 {
  for(StructNode &node : body->struct_list )
    {
     resetLevel();

     layout(&node);
    }

  for(ulen i=0,n=const_buf.getLen(); i<n ;i++)
    {
     auto &result=eval->const_table[i];

     resetLevel();

     const_buf[i].set(result.type,sizeOf(result.type).size_of);
    }
 }

ulen Map::reserve(ulen len)
 {
  if( !TryAlign(len) ) GuardMapLenOverflow();

  ulen ret=total;

  total=MapAddLen(total,len);

  return ret;
 }

ulen Map::reserve(ulen len,ulen count)
 {
  return reserve(MapMulLen(len,count));
 }

struct Map::PlaceFunc
 {
  Map *map;
  Value value;
  RecValue rec;

  PlaceFunc(Map *map_,const Value &value_,const RecValue &rec_) : map(map_),value(value_),rec(rec_) {}

  void operator () (TypeNode::Base *type_ptr)
   {
    if( type_ptr->isText() )
      {
       Text text=value.get<Text>();

       rec.elems->off=map->reserve(text.str.len);
      }
   }

  void operator () (TypeNode::Ptr *) {}

  void operator () (TypeNode::PolyPtr *) {}

  void array(ulen off,ulen delta,TypeNode *type)
   {
    PtrLen<Value> data=value.get<Block>().data;
    PtrLen<RecValue> elems=rec.elems;

    for(; +data ;++data,++elems,off+=delta)
      {
       elems->off=off;

       map->place(type,*data,*elems);
      }
   }

  void operator () (TypeNode::Array *type_ptr)
   {
    TypeNode *type=type_ptr->type_node;

    ulen delta=map->sizeOf(type).size_of;

    ulen off=map->reserve(delta,rec.elems.len);

    array(off,delta,type);
   }

  void operator () (TypeNode::ArrayLen *type_ptr)
   {
    TypeNode *type=type_ptr->type_node;

    ulen delta=map->sizeOf(type).size_of;

    array(rec.off,delta,type);
   }

  void operator () (StructNode *struct_node)
   {
    PtrLen<Value> data=value.get<Block>().data;

    ulen off=rec.off;
    PtrLen<RecValue> elems=rec.elems;

    auto cur=begin(struct_node->field_list);

    for(; +data ;++data,++elems,++cur)
      {
       elems->off=off+(*cur).index;

       map->place((*cur).type_node,*data,*elems);
      }
   }
 };

void Map::place(TypeNode *type,const Value &value,const RecValue &rec)
 {
  incLevel();

  TypeAdapter(type).ptr.apply(PlaceFunc(this,value,rec));

  decLevel();
 }

void Map::place()
 {
  total=0;

  for(ulen i=0,n=const_buf.getLen(); i<n ;i++)
    {
     reserve(const_buf[i]);
    }

  for(ulen i=0,n=const_buf.getLen(); i<n ;i++)
    {
     auto &result=eval->const_table[i];

     resetLevel();

     place(result.type,result.value,const_buf[i]);

     NameKey key(result.node);
     ConstAlgo::PrepareIns prepare(const_map,key);

     if( prepare.found )
       {
        GuardMapNameDuplication();
       }
     else
       {
        prepare.complete(&const_buf[i]);
       }
    }
 }

auto Map::getRec(PtrNode *node) -> const RecValue &
 {
  if( PtrNode *parent=node->parent )
    {
     return getRec(parent).elems[node->index];
    }
  else
    {
     return const_buf[node->index];
    }
 }

DataPtr Map::mapPtr(Ptr ptr)
 {
  if( !ptr || ptr.null ) return 0;

  void *place=base+getRec(ptr.ptr_node).off;

  return place;
 }

TypedDataPtr Map::mapTypedPtr(Ptr ptr,TypeList *type_list)
 {
  if( !ptr ) return TypedDataPtr();

  if( ptr.null ) return TypedDataPtr(0,TypeComparer(eval,MaxLevel).typeIndex(ptr.ptr_node->type,type_list));

  void *place=base+getRec(ptr.ptr_node).off;

  return TypedDataPtr(place,TypeComparer(eval,MaxLevel).typeIndex(ptr.ptr_node->type,type_list));
 }

struct Map::MapFunc
 {
  Map *map;
  Value value;
  RecValue rec;

  MapFunc(Map *map_,const Value &value_,const RecValue &rec_) : map(map_),value(value_),rec(rec_) {}

  template <class T,class ... SS>
  void place(SS && ... ss)
   {
    new(map->base+rec.off) T( std::forward<SS>(ss)... );
   }

  template <class IntType>
  void placeBase(IntType value)
   {
    place<typename IntType::ValueType>(value.value);
   }

  void placeBase(IP ip)
   {
    place<uint32>(ip.toInt());
   }

  void placeBase(Text text)
   {
    StrLen str=text.str;

    char *dst=map->base+rec.elems->off;

    str.copyTo(dst);

    place<StrLen>(dst,str.len);
   }

  template <class T>
  void op()
   {
    placeBase(value.get<T>());
   }

  void operator () (TypeNode::Base *type_ptr)
   {
    ExtBaseType(*this,type_ptr->type);
   }

  void operator () (TypeNode::Ptr *)
   {
    place<DataPtr>( map->mapPtr(value.get<Ptr>()) );
   }

  void operator () (TypeNode::PolyPtr *type_ptr)
   {
    place<TypedDataPtr>( map->mapTypedPtr(value.get<Ptr>(),type_ptr->type_list) );
   }

  void operator () (TypeNode::Array *type_ptr)
   {
    PtrLen<Value> data=value.get<Block>().data;
    TypeNode *type=type_ptr->type_node;

    if( !data )
      {
       place<ArrayPtr>();
      }
    else
      {
       PtrLen<RecValue> elems=rec.elems;

       place<ArrayPtr>(map->base+elems->off,data.len);

       for(; +data ;++data,++elems) map->map(type,*data,*elems);
      }
   }

  void operator () (TypeNode::ArrayLen *type_ptr)
   {
    PtrLen<Value> data=value.get<Block>().data;
    TypeNode *type=type_ptr->type_node;

    PtrLen<RecValue> elems=rec.elems;

    for(; +data ;++data,++elems) map->map(type,*data,*elems);
   }

  void operator () (StructNode *struct_node)
   {
    PtrLen<Value> data=value.get<Block>().data;

    PtrLen<RecValue> elems=rec.elems;

    auto cur=begin(struct_node->field_list);

    for(; +data ;++data,++elems,++cur) map->map((*cur).type_node,*data,*elems);
   }
 };

void Map::map(TypeNode *type,const Value &value,const RecValue &rec)
 {
  incLevel();

  TypeAdapter(type).ptr.apply(MapFunc(this,value,rec));

  decLevel();
 }

void Map::SetDesc(const TypeDesc_struct_base &desc,RecStruct *rec)
 {
  desc.size_of=rec->size_of;

  auto cur=begin(rec->struct_node->field_list);

  for(auto r=Range(desc.fields,desc.field_count); +r ;++r,++cur) r->off=(*cur).index;
 }

struct Map::MatchFunc
 {
  Map *map;
  bool &ret;
  const TypeDesc &desc;

  MatchFunc(Map *map_,bool &ret_,const TypeDesc &desc_) : map(map_),ret(ret_),desc(desc_) {}

  static TypeTag ToTag(TypeNode::Base::Type type)
   {
    switch( type )
      {
       case TypeNode::Base::Type_sint   : return TypeTag_sint;
       case TypeNode::Base::Type_uint   : return TypeTag_uint;
       case TypeNode::Base::Type_ulen   : return TypeTag_ulen;

       case TypeNode::Base::Type_sint8  : return TypeTag_sint8;
       case TypeNode::Base::Type_uint8  : return TypeTag_uint8;
       case TypeNode::Base::Type_sint16 : return TypeTag_sint16;
       case TypeNode::Base::Type_uint16 : return TypeTag_uint16;
       case TypeNode::Base::Type_sint32 : return TypeTag_sint32;
       case TypeNode::Base::Type_uint32 : return TypeTag_uint32;
       case TypeNode::Base::Type_sint64 : return TypeTag_sint64;
       case TypeNode::Base::Type_uint64 : return TypeTag_uint64;

       case TypeNode::Base::Type_text   : return TypeTag_text;
       case TypeNode::Base::Type_ip     : return TypeTag_ip;

       default: return TypeTag_none;
      }
   }

  void operator () (TypeNode::Base *type_ptr)
   {
    ret = ( desc.tag==ToTag(type_ptr->type) ) ;
   }

  void operator () (TypeNode::Ptr *type_ptr)
   {
    if( desc.tag==TypeTag_ptr )
      {
       ret=map->match(type_ptr->type_node,static_cast<const TypeDesc_ptr &>(desc).type);
      }
    else
      {
       ret=false;
      }
   }

  bool match(const TypeDesc_polyptr_base &desc,TypeList *type_list)
   {
    ulen ind=desc.type_count;

    if( ind!=type_list->count ) return false;

    for(TypeListNode &node : *type_list )
      {
       --ind;

       if( !map->match(node.type_node,desc.types[ind].type) ) return false;
      }

    return true;
   }

  void operator () (TypeNode::PolyPtr *type_ptr)
   {
    if( desc.tag==TypeTag_polyptr )
      {
       ret=match(static_cast<const TypeDesc_polyptr_base &>(desc),type_ptr->type_list);
      }
    else
      {
       ret=false;
      }
   }

  void operator () (TypeNode::Array *type_ptr)
   {
    if( desc.tag==TypeTag_array )
      {
       ret=map->match(type_ptr->type_node,static_cast<const TypeDesc_array &>(desc).type);
      }
    else
      {
       ret=false;
      }
   }

  void operator () (TypeNode::ArrayLen *type_ptr)
   {
    switch( desc.tag )
      {
       case TypeTag_array_len :
        {
         const TypeDesc_array_len &updesc=static_cast<const TypeDesc_array_len &>(desc);

         if( map->eval->getLen(type_ptr->len_node)==updesc.len )
           {
            ret=map->match(type_ptr->type_node,updesc.type);
           }
         else
           {
            ret=false;
           }
        }
       break;

       case TypeTag_array_getlen :
        {
         const TypeDesc_array_getlen &updesc=static_cast<const TypeDesc_array_getlen &>(desc);

         updesc.len=map->eval->getLen(type_ptr->len_node);

         ret=map->match(type_ptr->type_node,updesc.type);
        }
       break;

       default:
        {
         ret=false;
        }
      }
   }

  void operator () (StructNode *struct_node)
   {
    if( desc.tag==TypeTag_struct )
      {
       const TypeDesc_struct_base &updesc=static_cast<const TypeDesc_struct_base &>(desc);

       if( updesc.lock )
         {
          ret=( updesc.lock==struct_node->index+1 );

          return;
         }

       updesc.lock=struct_node->index+1;

       if( updesc.field_count==struct_node->field_list.count )
         {
          auto *ptr=updesc.fields;

          for(FieldNode &field_node : struct_node->field_list )
            {
             if( !map->match(field_node.type_node,ptr->type) )
               {
                updesc.lock=0;

                ret=false;

                return;
               }

             ptr->off=field_node.index;

             ++ptr;
            }

          updesc.size_of=map->struct_buf[struct_node->index].size_of;

          ret=true;
         }
       else
         {
          updesc.lock=0;

          ret=false;
         }
      }
    else
      {
       ret=false;
      }
   }
 };

bool Map::match(TypeNode *type,const TypeDesc &desc)
 {
  bool ret=false;

  TypeAdapter(type).ptr.apply(MatchFunc(this,ret,desc));

  return ret;
 }

bool Map::match(StructNode *struct_node,const TypeDesc &desc)
 {
  bool ret=false;

  MatchFunc(this,ret,desc)(struct_node);

  return ret;
 }

Map::Map(EngineResult result)
 : eval(result.eval),
   body(result.body),
   const_buf(result.eval->const_table.getLen()),
   struct_buf(result.body->struct_list.count),
   total(0),
   base(0),
   level(0)
 {
  prepare();

  sizeOf();

  place();
 }

Map::~Map()
 {
 }

void Map::operator () (void *mem)
 {
  base=Place<void>(mem);

  for(ulen i=0,n=const_buf.getLen(); i<n ;i++)
    {
     auto &result=eval->const_table[i];

     resetLevel();

     map(result.type,result.value,const_buf[i]);
    }
 }

bool Map::findStruct(StrLen name,const TypeDesc &desc)
 {
  NameKey key(name);

  RecStruct *rec=struct_map.find(key);

  if( !rec ) return false;

  if( !match(rec->struct_node,desc) ) return false;

  SetDesc(static_cast<const TypeDesc_struct_base &>(desc),rec);

  return true;
 }

void * Map::findConst(StrLen name,const TypeDesc &desc)
 {
  NameKey key(name);

  RecConst *rec=const_map.find(key);

  if( !rec ) return 0;

  if( !match(rec->type,desc) ) return 0;

  return base+rec->off;
 }

} // namespace DDL
} // namespace CCore


