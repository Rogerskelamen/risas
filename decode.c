#include "decode.h"
#include <stdlib.h>

int
decode(int inst_id, INSTVAR *v)
{
  INST *cinst;
  if ((cinst = (INST *)malloc(sizeof(INST))) == NULL)     return 1;

  get_data(inst_id, &cinst);
  switch (get_type(inst_id)) {
    case TYPE_R:
      if(dec_r(cinst, v))
        return 1;
      break;
    case TYPE_I:
      if (par_i(code, v))
        return 1;
      break;
    case TYPE_S:
      if (par_s(code, v))
        return 1;
      break;
    case TYPE_B:
      if (par_b(code, v, tag_imm))
        return 1;
      break;
    case TYPE_U:
      if (par_u(code, v))
        return 1;
      break;
    case TYPE_J:
      if (par_j(code, v, tag_imm))
        return 1;
      break;
    case TYPE_N:
      if (par_n(code))
        return 1;
      break;
    default:
      return 1; // can't find the type
      break;
  }
  return 0;
}

int
dec_r(INST *inst, INSTVAR *v)
{

}
