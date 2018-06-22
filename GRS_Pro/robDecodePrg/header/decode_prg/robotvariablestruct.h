#ifndef ROBOTVARIABLESTRUCT_H
#define ROBOTVARIABLESTRUCT_H

#include <QList>

#include "decode_typestructs.h"

#if defined(__cplusplus)
extern "C"
{
#endif

typedef struct {
    int varType;//变量类型
    int valIndex;//变量值数组下标索引
    decode_type_property typePro;
}robot_variable;

typedef struct {
    QList<robot_variable> robotVar;
}robot_var_struct;



#if defined(__cplusplus)
}
#endif

#endif // ROBOTVARIABLESTRUCT_H
