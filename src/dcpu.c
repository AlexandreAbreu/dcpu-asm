#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <assert.h>

#include "debugger/command_parser.h"
#include "debugger/debugger.h"


typedef uint16_t word;

// in words
#define RAM_SIZE 0x10000
#define REGISTER_COUNT 8

typedef struct dcpu_t_
{
  word pc;
  word sp;
  word o;
  word registers [REGISTER_COUNT];
  word ram [RAM_SIZE];
  
} dcpu_t;


typedef word (*ValueConsumerFunc) (void);
typedef void (* NextInstructionFunc) (dcpu_t * cpu
				      , ValueConsumerFunc next_value);


typedef word EncodedValue;

#define DCPU_INST_OPCODE_MASK  0x000F
#define DCPU_INST_OPERAND_MASK 0xFFF0
#define DCPU_INST_A_MASK 0x03F0
#define DCPU_INST_B_MASK 0xFC00


void
execute_jsr (dcpu_t * cpu
	     , EncodedValue evalue_a
	     , EncodedValue evalue_b
	     , ValueConsumerFunc next_value
	     , NextInstructionFunc next_instruction);

void
execute_set (dcpu_t * cpu
	     , EncodedValue evalue_a
	     , EncodedValue evalue_b
	     , ValueConsumerFunc next_value
	     , NextInstructionFunc next_instruction);

void
execute_add (dcpu_t * cpu
	     , EncodedValue evalue_a
	     , EncodedValue evalue_b
	     , ValueConsumerFunc next_value
	     , NextInstructionFunc next_instruction);

void
execute_sub (dcpu_t * cpu
	     , EncodedValue evalue_a
	     , EncodedValue evalue_b
	     , ValueConsumerFunc next_value
	     , NextInstructionFunc next_instruction);

void
execute_mul (dcpu_t * cpu
	     , EncodedValue evalue_a
	     , EncodedValue evalue_b
	     , ValueConsumerFunc next_value
	     , NextInstructionFunc next_instruction);

void
execute_div (dcpu_t * cpu
	     , EncodedValue evalue_a
	     , EncodedValue evalue_b
	     , ValueConsumerFunc next_value
	     , NextInstructionFunc next_instruction);

void
execute_mod (dcpu_t * cpu
	     , EncodedValue evalue_a
	     , EncodedValue evalue_b
	     , ValueConsumerFunc next_value
	     , NextInstructionFunc next_instruction);

void
execute_shl (dcpu_t * cpu
	     , EncodedValue evalue_a
	     , EncodedValue evalue_b
	     , ValueConsumerFunc next_value
	     , NextInstructionFunc next_instruction);

void
execute_shr (dcpu_t * cpu
	     , EncodedValue evalue_a
	     , EncodedValue evalue_b
	     , ValueConsumerFunc next_value
	     , NextInstructionFunc next_instruction);

void
execute_and (dcpu_t * cpu
	     , EncodedValue evalue_a
	     , EncodedValue evalue_b
	     , ValueConsumerFunc next_value
	     , NextInstructionFunc next_instruction);

void
execute_bor (dcpu_t * cpu
	     , EncodedValue evalue_a
	     , EncodedValue evalue_b
	     , ValueConsumerFunc next_value
	     , NextInstructionFunc next_instruction);

void
execute_xor (dcpu_t * cpu
	     , EncodedValue evalue_a
	     , EncodedValue evalue_b
	     , ValueConsumerFunc next_value
	     , NextInstructionFunc next_instruction);

void
execute_ife (dcpu_t * cpu
	     , EncodedValue evalue_a
	     , EncodedValue evalue_b
	     , ValueConsumerFunc next_value
	     , NextInstructionFunc next_instruction);

void
execute_ifn (dcpu_t * cpu
	     , EncodedValue evalue_a
	     , EncodedValue evalue_b
	     , ValueConsumerFunc next_value
	     , NextInstructionFunc next_instruction);

void
execute_ifg (dcpu_t * cpu
	     , EncodedValue evalue_a
	     , EncodedValue evalue_b
	     , ValueConsumerFunc next_value
	     , NextInstructionFunc next_instruction);

void
execute_ifb (dcpu_t * cpu
	     , EncodedValue evalue_a
	     , EncodedValue evalue_b
	     , ValueConsumerFunc next_value
	     , NextInstructionFunc next_instruction);


/*typedef enum operand_t_
  {
    
  } operant_t;*/

unsigned char extract_opcode (word w)
{
  return w & DCPU_INST_OPCODE_MASK;
}
unsigned char extract_a (word w)
{
  return (w & DCPU_INST_A_MASK) >> 4;
}
unsigned char extract_b (word w)
{
  return (w & DCPU_INST_B_MASK) >> 10;
}
  
typedef enum opcode_inst_t_
  {
    OPCODE_BASIC = 0x0,
    OPCODE_SET = 0x1,
    OPCODE_ADD = 0x2,
    OPCODE_SUB = 0x3,
    OPCODE_MUL = 0x4,
    OPCODE_DIV = 0x5,
    OPCODE_MOD = 0x6,
    OPCODE_SHL = 0x7,
    OPCODE_SHR = 0x8,
    OPCODE_AND = 0x9,
    OPCODE_BOR = 0xa,
    OPCODE_XOR = 0xb,
    OPCODE_IFE = 0xc,
    OPCODE_IFN = 0xd,
    OPCODE_IFG = 0xe,
    OPCODE_IFB = 0xf
    
  } opcode_inst_t;


typedef void (* OpcodeExecute) (dcpu_t * cpu
				, EncodedValue evalue_a
				, EncodedValue evalue_b
				, ValueConsumerFunc next_value
				, NextInstructionFunc next_instruction);


typedef struct opcode_t_
{
  opcode_inst_t inst;
  char * name;
  OpcodeExecute execute;
  
} opcode_t;

#define DEFINE_OPCODE(base_inst,execute_func) \
  { \
    .inst = OPCODE_ ## base_inst, \
    .name = #base_inst \
    , .execute = execute_func \
  }

opcode_t
opcodes [] = {
  DEFINE_OPCODE(BASIC,NULL)
  ,
  DEFINE_OPCODE(SET,execute_set)
  ,
  DEFINE_OPCODE(ADD,execute_add)
  ,
  DEFINE_OPCODE(SUB,execute_sub)
  ,
  DEFINE_OPCODE(MUL,execute_mul)
  ,
  DEFINE_OPCODE(DIV,execute_div)
  ,
  DEFINE_OPCODE(MOD,execute_mod)
  ,
  DEFINE_OPCODE(SHL,execute_shl)
  ,
  DEFINE_OPCODE(SHR,execute_shr)
  ,
  DEFINE_OPCODE(AND,execute_and)
  ,
  DEFINE_OPCODE(BOR,execute_bor)
  ,
  DEFINE_OPCODE(XOR,execute_xor)
  ,
  DEFINE_OPCODE(IFE,execute_ife)
  ,
  DEFINE_OPCODE(IFN,execute_ifn)
  ,
  DEFINE_OPCODE(IFG,execute_ifg)
  ,
  DEFINE_OPCODE(IFB,execute_ifb)
};


const char *
stringify_value (word value, ValueConsumerFunc next_value)
{
  // TODO assert consumer
  
  const char * register_from (unsigned char r)
  {
    static char * register_names [] = {
      "A", "B", "C", "X", "Y", "Z", "I", "J"
    };
    return register_names[r & 0x3F];
  }
  
  if (value <= 0x07)
    {
      return strdup (register_from (value));
    }
  if (value <= 0x0f)
    {
      char v[3+1] = {0};
      snprintf (&v[0], sizeof(v) / sizeof (v[0]), "[%s]", register_from (value - 0x08));
      return strdup (v);
    }
  if (value <= 0x17)
    {
      word next = next_value ();
      
      char v[64] = {0};
      snprintf (&v[0], sizeof(v) / sizeof (v[0]), "[0x%04X + %s]"
		, next
		, register_from (value - 0x0f));
      return strdup (v);
    }
  if (value == 0x18)
    {
      return strdup ("POP");
    }
  if (value == 0x19)
    {
      return strdup ("PEEK");
    }
  if (value == 0x1a)
    {
      return strdup ("PUSH");
    }
  if (value == 0x1b)
    {
      return strdup ("SP");
    }
  if (value == 0x1c)
    {
      return strdup ("PC");
    }
  if (value == 0x1d)
    {
      return strdup ("O");
    }
  if (value == 0x1e)
    {
      word next = next_value ();
      
      char v[64] = {0};
      snprintf (&v[0], sizeof(v) / sizeof (v[0]), "[0x%04X]"
		, next);
      return strdup (v);
    }
  if (value == 0x1f)
    {
      word next = next_value ();
      
      char v[64] = {0};
      snprintf (&v[0], sizeof(v) / sizeof (v[0]), "0x%04X"
		, next);
      return strdup (v);
    }
  if (value >= 0x20)
    {
      char v[64] = {0};
      snprintf (&v[0], sizeof(v) / sizeof (v[0]), "0x%04X"
		, value - 0x20);
      return strdup (v);
    }
}


char *
stringify_instruction (word value, ValueConsumerFunc next_value)
{
  unsigned char opcode = extract_opcode (value);
  
  char stringified[64] = {0};
  if (0 == opcode)
    {
      // handled as a special case
      word code = extract_a (value);
      const char * value_a = stringify_value (extract_b (value), next_value);
      
      snprintf (stringified
		, sizeof(stringified) / sizeof(stringified[0])
		, "%s %s"
		, code == 0x01 ? "JSR" : "UNKNOWN"
		, value_a);
      
      free((void *) value_a);
    }
  else
    {
      const char * value_a = stringify_value (extract_a (value), next_value);
      const char * value_b = stringify_value (extract_b (value), next_value);
      snprintf (stringified
		, sizeof(stringified) / sizeof(stringified[0])
		, "%s %s, %s"
		, opcodes[opcode].name
		, value_a
		, value_b);
      free((void *) value_a);
      free((void *) value_b);
    }
  
  return strdup (stringified);
}


typedef enum TaggedValueType_t
  {
    UNKNOWN_VALUE,
    PLAIN_VALUE,
    MEMORY_REFERENCE,
    DCPU_REFERENCE
    
  } TaggedValueType;

typedef struct TaggedValue_t
{
  TaggedValueType type;
  word value;
  
} TaggedValue;


bool
is_assignable (TaggedValue tvalue)
{
  return tvalue.type == MEMORY_REFERENCE || tvalue.type == DCPU_REFERENCE;
}


void
assign_to_tagged_value (dcpu_t * cpu, TaggedValue tvalue, word value)
{
  if ( ! is_assignable (tvalue))
    {
      // TODO fallback mode - signal error
      return;
    }
  
  switch (tvalue.type)
    {
    case MEMORY_REFERENCE:
      // TODO validate memory assignment
      cpu->ram[tvalue.value] = value;
      break;
      
    case DCPU_REFERENCE:
      // TODO validate this stuff
      printf ("Assigning offset %d - value 0x%04X, %d\n", tvalue.value, value, offsetof(dcpu_t,pc));
      *((word *) ((char*) cpu + tvalue.value)) = value;
      break;
      
    defaut:
      // should not happen
      // TODO but signal if it does
      break;
    }
}


word
value_from_tagged_value (dcpu_t * cpu, TaggedValue tvalue)
{
  word value = 0;
  
  switch (tvalue.type)
    {
    case MEMORY_REFERENCE:
      // TODO validate memory assignment
      value = cpu->ram[tvalue.value];
      break;
      
    case DCPU_REFERENCE:
      // TODO validate this stuff
      value = *((word *) ((char*) cpu + tvalue.value));
      break;
      
    case PLAIN_VALUE:
      value = tvalue.value;
      break;
    }
  
  return value;
}


TaggedValue
decode_value (dcpu_t * cpu, word value, ValueConsumerFunc next_value)
{
  TaggedValue tvalue = {.type = UNKNOWN_VALUE, .value = 0};
  
  if (value <= 0x07)
    {
      tvalue.type = DCPU_REFERENCE;
      tvalue.value = (word) (offsetof (dcpu_t, registers)
			     + value * sizeof(cpu->registers[0]));
      
      return tvalue;
    }
  if (value <= 0x0f)
    {
      // TODO add ram validation checks & fallback
      tvalue.type = MEMORY_REFERENCE;
      tvalue.value = cpu->registers[value - 0x08];
      
      return tvalue;
    }
  if (value <= 0x17)
    {
      // TODO add ram validation checks & fallback
      word next = next_value ();
      tvalue.type = MEMORY_REFERENCE;
      tvalue.value = cpu->registers[value - 0x0f] + next;
      
      return tvalue;
    }
  if (value == 0x18)
    {
      // TODO add stack validation checks & fallback
      tvalue.type = MEMORY_REFERENCE;
      tvalue.value = cpu->sp++;
      
      return tvalue;
    }
  if (value == 0x19)
    {
      // TODO add stack validation checks & fallback
      tvalue.type = MEMORY_REFERENCE;
      tvalue.value = cpu->sp;
      
      return tvalue;
    }
  if (value == 0x1a)
    {
      // TODO add stack validation checks & fallback
      tvalue.type = MEMORY_REFERENCE;
      tvalue.value = cpu->sp--;
      
      return tvalue;
    }
  if (value == 0x1b)
    {
      // TODO add stack validation checks & fallback
      tvalue.type = DCPU_REFERENCE;
      // dangerous ...
      tvalue.value = (word) offsetof (dcpu_t, sp);
      
      return tvalue;
    }
  if (value == 0x1c)
    {
      // TODO add stack validation checks & fallback
      tvalue.type = DCPU_REFERENCE;
      // dangerous ...
      tvalue.value = (word) offsetof (dcpu_t, pc);
      printf ("Tagged value PC: %d\n", tvalue.value);
      
      return tvalue;
    }
  if (value == 0x1d)
    {
      // TODO add stack validation checks & fallback
      tvalue.type = DCPU_REFERENCE;
      // dangerous ...
      tvalue.value = (word) offsetof (dcpu_t, o);
      
      return tvalue;
    }
  if (value == 0x1e)
    {
      // TODO add ram validation checks & fallback
      word next = next_value ();
      tvalue.type = MEMORY_REFERENCE;
      tvalue.value = next;
      
      return tvalue;
    }
  if (value == 0x1f)
    {
      word next = next_value ();
      tvalue.type = PLAIN_VALUE;
      tvalue.value = next;
      
      return tvalue;
    }
  if (value >= 0x20)
    {
      tvalue.type = PLAIN_VALUE;
      tvalue.value = value - 0x20;
      
      return tvalue;
    }
  
  return tvalue;
}


// TODO refactor & group w/ stringify & execute
void
next_instruction (dcpu_t * cpu, ValueConsumerFunc next_value)
{
  word value = next_value ();
  
  unsigned char opcode = extract_opcode (value);
  
  if (0 == opcode)
    {
      // handled as a special case
      word code = extract_a (value);
      decode_value (cpu, extract_b (value), next_value);
    }
  else
    {
      TaggedValue tvalue_a = decode_value (cpu, extract_a (value), next_value);
      TaggedValue tvalue_b = decode_value (cpu, extract_b (value), next_value);
    }
}


void
execute_jsr (dcpu_t * cpu
	     , EncodedValue evalue_a
	     , EncodedValue evalue_b
	     , ValueConsumerFunc next_value
	     , NextInstructionFunc next_instruction)
{
  // TODO add stack validation
  cpu->ram[cpu->sp--] = cpu->pc + 1;
  cpu->pc = value_from_tagged_value (cpu, decode_value (cpu, evalue_a, next_value));
}


void
execute_set (dcpu_t * cpu
	     , EncodedValue evalue_a
	     , EncodedValue evalue_b
	     , ValueConsumerFunc next_value
	     , NextInstructionFunc next_instruction)
{
  // we want to preserve the eval order, value 'a' then 'b'
  TaggedValue tvalue_a = decode_value (cpu, evalue_a, next_value);
  TaggedValue tvalue_b = decode_value (cpu, evalue_b, next_value);
  
  assign_to_tagged_value (cpu, tvalue_a, value_from_tagged_value (cpu, tvalue_b));
}


void
execute_add (dcpu_t * cpu
	     , EncodedValue evalue_a
	     , EncodedValue evalue_b
	     , ValueConsumerFunc next_value
	     , NextInstructionFunc next_instruction)
{
  // we want to preserve the eval order, value 'a' then 'b'
  TaggedValue tvalue_a = decode_value (cpu, evalue_a, next_value);
  TaggedValue tvalue_b = decode_value (cpu, evalue_b, next_value);
  
  word value_a = value_from_tagged_value (cpu, tvalue_a);
  word value_b = value_from_tagged_value (cpu, tvalue_b);
  
  assign_to_tagged_value (cpu, tvalue_a, (value_a + value_b) % 65536);
  if (value_a + value_b > 0xFFFF)
    {
      cpu->o = 0x01;
    }
  else
    {
      cpu->o = 0x00;
    }
}


void
execute_sub (dcpu_t * cpu
	     , EncodedValue evalue_a
	     , EncodedValue evalue_b
	     , ValueConsumerFunc next_value
	     , NextInstructionFunc next_instruction)
{
  // we want to preserve the eval order, value 'a' then 'b'
  TaggedValue tvalue_a = decode_value (cpu, evalue_a, next_value);
  TaggedValue tvalue_b = decode_value (cpu, evalue_b, next_value);
  
  word value_a = value_from_tagged_value (cpu, tvalue_a);
  word value_b = value_from_tagged_value (cpu, tvalue_b);
  
  assign_to_tagged_value (cpu, tvalue_a, (value_a - value_b) % 65536);
  if (value_a - value_b < 0)
    {
      cpu->o = 0xFFFF;
    }
  else
    {
      cpu->o = 0x00;
    }
}


void
execute_mul (dcpu_t * cpu
	     , EncodedValue evalue_a
	     , EncodedValue evalue_b
	     , ValueConsumerFunc next_value
	     , NextInstructionFunc next_instruction)
{
  // we want to preserve the eval order, value 'a' then 'b'
  TaggedValue tvalue_a = decode_value (cpu, evalue_a, next_value);
  TaggedValue tvalue_b = decode_value (cpu, evalue_b, next_value);
  
  word value_a = value_from_tagged_value (cpu, tvalue_a);
  word value_b = value_from_tagged_value (cpu, tvalue_b);
  
  assign_to_tagged_value (cpu, tvalue_a, (value_a * value_b) % 65536);
  
  cpu->o = ((value_a * value_b) >> 16) & 0xffff;
}


void
execute_div (dcpu_t * cpu
	     , EncodedValue evalue_a
	     , EncodedValue evalue_b
	     , ValueConsumerFunc next_value
	     , NextInstructionFunc next_instruction)
{
  // we want to preserve the eval order, value 'a' then 'b'
  TaggedValue tvalue_a = decode_value (cpu, evalue_a, next_value);
  TaggedValue tvalue_b = decode_value (cpu, evalue_b, next_value);
  
  word value_a = value_from_tagged_value (cpu, tvalue_a);
  word value_b = value_from_tagged_value (cpu, tvalue_b);
  
  if (0 != value_b)
    {
      assign_to_tagged_value (cpu, tvalue_a, (value_a / value_b) % 65536);
      cpu->o = ((value_a << 16) / value_b) & 0xffff;
    }
  else
    {
      assign_to_tagged_value (cpu, tvalue_a, 0);
      cpu->o = 0;
    }
}


void
execute_mod (dcpu_t * cpu
	     , EncodedValue evalue_a
	     , EncodedValue evalue_b
	     , ValueConsumerFunc next_value
	     , NextInstructionFunc next_instruction)
{
  // we want to preserve the eval order, value 'a' then 'b'
  TaggedValue tvalue_a = decode_value (cpu, evalue_a, next_value);
  TaggedValue tvalue_b = decode_value (cpu, evalue_b, next_value);
  
  word value_a = value_from_tagged_value (cpu, tvalue_a);
  word value_b = value_from_tagged_value (cpu, tvalue_b);
  
  if (0 != value_b)
    {
      assign_to_tagged_value (cpu, tvalue_a, (value_a % value_b));
    }
  else
    {
      assign_to_tagged_value (cpu, tvalue_a, 0);
    }
}


void
execute_shl (dcpu_t * cpu
	     , EncodedValue evalue_a
	     , EncodedValue evalue_b
	     , ValueConsumerFunc next_value
	     , NextInstructionFunc next_instruction)
{
  // we want to preserve the eval order, value 'a' then 'b'
  TaggedValue tvalue_a = decode_value (cpu, evalue_a, next_value);
  TaggedValue tvalue_b = decode_value (cpu, evalue_b, next_value);
  
  word value_a = value_from_tagged_value (cpu, tvalue_a);
  word value_b = value_from_tagged_value (cpu, tvalue_b);
  
  assign_to_tagged_value (cpu, tvalue_a, (value_a << value_b));
  cpu->o = ((value_a << value_b) >> 16) & 0xffff;
}


void
execute_shr (dcpu_t * cpu
	     , EncodedValue evalue_a
	     , EncodedValue evalue_b
	     , ValueConsumerFunc next_value
	     , NextInstructionFunc next_instruction)
{
  // we want to preserve the eval order, value 'a' then 'b'
  TaggedValue tvalue_a = decode_value (cpu, evalue_a, next_value);
  TaggedValue tvalue_b = decode_value (cpu, evalue_b, next_value);
  
  word value_a = value_from_tagged_value (cpu, tvalue_a);
  word value_b = value_from_tagged_value (cpu, tvalue_b);
  
  assign_to_tagged_value (cpu, tvalue_a, (value_a >> value_b));
  cpu->o = ((value_a << 16) >> value_b) & 0xffff;
}


void
execute_and (dcpu_t * cpu
	     , EncodedValue evalue_a
	     , EncodedValue evalue_b
	     , ValueConsumerFunc next_value
	     , NextInstructionFunc next_instruction)
{
  // we want to preserve the eval order, value 'a' then 'b'
  TaggedValue tvalue_a = decode_value (cpu, evalue_a, next_value);
  TaggedValue tvalue_b = decode_value (cpu, evalue_b, next_value);
  
  word value_a = value_from_tagged_value (cpu, tvalue_a);
  word value_b = value_from_tagged_value (cpu, tvalue_b);
  
  assign_to_tagged_value (cpu, tvalue_a, value_a & value_b);
}


void
execute_bor (dcpu_t * cpu
	     , EncodedValue evalue_a
	     , EncodedValue evalue_b
	     , ValueConsumerFunc next_value
	     , NextInstructionFunc next_instruction)
{
  // we want to preserve the eval order, value 'a' then 'b'
  TaggedValue tvalue_a = decode_value (cpu, evalue_a, next_value);
  TaggedValue tvalue_b = decode_value (cpu, evalue_b, next_value);
  
  word value_a = value_from_tagged_value (cpu, tvalue_a);
  word value_b = value_from_tagged_value (cpu, tvalue_b);
  
  assign_to_tagged_value (cpu, tvalue_a, value_a | value_b);
}


void
execute_xor (dcpu_t * cpu
	     , EncodedValue evalue_a
	     , EncodedValue evalue_b
	     , ValueConsumerFunc next_value
	     , NextInstructionFunc next_instruction)
{
  // we want to preserve the eval order, value 'a' then 'b'
  TaggedValue tvalue_a = decode_value (cpu, evalue_a, next_value);
  TaggedValue tvalue_b = decode_value (cpu, evalue_b, next_value);
  
  word value_a = value_from_tagged_value (cpu, tvalue_a);
  word value_b = value_from_tagged_value (cpu, tvalue_b);
  
  assign_to_tagged_value (cpu, tvalue_a, value_a ^ value_b);
}


void
execute_ife (dcpu_t * cpu
	     , EncodedValue evalue_a
	     , EncodedValue evalue_b
	     , ValueConsumerFunc next_value
	     , NextInstructionFunc next_instruction)
{
  // we want to preserve the eval order, value 'a' then 'b'
  TaggedValue tvalue_a = decode_value (cpu, evalue_a, next_value);
  TaggedValue tvalue_b = decode_value (cpu, evalue_b, next_value);
  
  word value_a = value_from_tagged_value (cpu, tvalue_a);
  word value_b = value_from_tagged_value (cpu, tvalue_b);
  
  if (value_a != value_b)
    {
      // skip next instruction
      next_instruction(cpu, next_value);
    }
}


void
execute_ifn (dcpu_t * cpu
	     , EncodedValue evalue_a
	     , EncodedValue evalue_b
	     , ValueConsumerFunc next_value
	     , NextInstructionFunc next_instruction)
{
  // we want to preserve the eval order, value 'a' then 'b'
  TaggedValue tvalue_a = decode_value (cpu, evalue_a, next_value);
  TaggedValue tvalue_b = decode_value (cpu, evalue_b, next_value);
  
  word value_a = value_from_tagged_value (cpu, tvalue_a);
  word value_b = value_from_tagged_value (cpu, tvalue_b);
  
  if (value_a == value_b)
    {
      // skip next instruction
      next_instruction(cpu, next_value);
    }
}


void
execute_ifg (dcpu_t * cpu
	     , EncodedValue evalue_a
	     , EncodedValue evalue_b
	     , ValueConsumerFunc next_value
	     , NextInstructionFunc next_instruction)
{
  // we want to preserve the eval order, value 'a' then 'b'
  TaggedValue tvalue_a = decode_value (cpu, evalue_a, next_value);
  TaggedValue tvalue_b = decode_value (cpu, evalue_b, next_value);
  
  word value_a = value_from_tagged_value (cpu, tvalue_a);
  word value_b = value_from_tagged_value (cpu, tvalue_b);
  
  if (value_a <= value_b)
    {
      // skip next instruction
      next_instruction(cpu, next_value);
    }
}


void
execute_ifb (dcpu_t * cpu
	     , EncodedValue evalue_a
	     , EncodedValue evalue_b
	     , ValueConsumerFunc next_value
	     , NextInstructionFunc next_instruction)
{
  // we want to preserve the eval order, value 'a' then 'b'
  TaggedValue tvalue_a = decode_value (cpu, evalue_a, next_value);
  TaggedValue tvalue_b = decode_value (cpu, evalue_b, next_value);
  
  word value_a = value_from_tagged_value (cpu, tvalue_a);
  word value_b = value_from_tagged_value (cpu, tvalue_b);
  
  if (0 == (value_a & value_b))
    {
      // skip next instruction
      next_instruction(cpu, next_value);
    }
}


void
execute_instruction (dcpu_t * cpu
		     , word value
		     , ValueConsumerFunc next_word
		     , NextInstructionFunc next_instruction)
{
  unsigned char opcode = extract_opcode (value);
  if (0 == opcode)
    {
      // handled as a special case
      word code = extract_a (value);
      word value_a = extract_b (value);
      
      if (code == 0x01)
	{
	  execute_jsr (cpu, value_a, 0, next_word, next_instruction);
	}
    }
  else
    {
      opcodes[opcode].execute (cpu
			       , extract_a (value)
			       , extract_b (value)
			       , next_word
			       , next_instruction);
    }
}


void run_vm_with (dcpu_t * cpu
		  , word program []
		  , size_t psize
		  , debugger_t * debugger)
{
  cpu->pc = 0;
  cpu->sp = RAM_SIZE - 1;
  
  word next_word (void)
  {
    return program [cpu->pc++];
  }
  
  
  while (1)
    {
      execute_instruction (cpu
			   , next_word ()
			   , next_word
			   , next_instruction);
    }
}


// @param size in words
void disassemble (word program [], size_t size)
{
  unsigned int pc = 0;
  
  word next_value (void)
  {
    return program[pc++];
  }
  
  for (; pc < size; )
    {
      word curpc = pc;
      
      word value = next_value ();
      char * stringified = stringify_instruction (value, next_value);
      printf ("0x%08X: %s ", curpc, stringified);
      free((void *) stringified);
      
      printf ("\n");
    }
}


void run_with_debugger (word program[]
			, size_t size
			, dcpu_t * cpu)
{
  cpu->pc = 0;
  cpu->sp = RAM_SIZE - 1;
  
  word next_word (void)
  {
    return cpu->ram[cpu->pc++];
  }
  
  int should_be_stopped (dcpu_t * cpu
			 , const char * const arguments)
  {
    int value_from_symbol_name (const char * name)
    {
      // static list of symbols
      if (0 == strncasecmp (name, "IP", strlen(name)))
	{
	  return cpu->pc;
	}
      return -1;
    }
    
    // try to parse
    environment_t
      env = {
      .get_symbol_value = value_from_symbol_name
    };
    
    int result = execute_command ((const char *) arguments, env);
    if (result < 0)
      {
	printf ("Could not properly parse: %s\n", arguments);
	return 1;
      }
    
    return result;
  }
  
  int peek_next ()
  {
    // assume that the only side effect is PC
    // TODO refactor this
    word pc = cpu->pc;
    char * stringified = stringify_instruction (next_word(), next_word);
    printf ("0x%08X: %s\n", pc, stringified != NULL ? stringified : "??");
    free((void *) stringified);
    cpu->pc = pc;
    
    return 0;
  }
  
  int next ()
  {
    peek_next ();
    
    execute_instruction (cpu
			 , next_word()
			 , next_word
			 , next_instruction);
    
    return 0;
  }
  
  int run_until (const char * const arguments)
  {
    while (0 == should_be_stopped (cpu, arguments))
      {
	next ();
      }
  }
  
  int where ()
  {
    printf ("PC: 0x%08X\n", cpu->pc);
    return 0;
  }
  
  int registers ()
  {
    unsigned char i = 0;
    for (i = 0; i < REGISTER_COUNT; ++i)
      {
	assert (i < (sizeof(cpu->registers) / sizeof(cpu->registers[0])));
	printf ("reg[%d] = 0x%08X\n", i, cpu->registers[i]);
      }
    return 0;
  }

  debugger_t debugger = {
    .next = next,
    .where = where,
    .registers = registers,
    .run_until = run_until,
    .peek_next = peek_next
  };
  
  memcpy (cpu->ram, program, size);
  
  run_debugger (&debugger);
  
  /*run_vm_with (cpu
	       , program
	       , sizeof(program) / sizeof(program[0])
	       , &debugger);*/
}



int main (void)
{
  word program [] = {
    0x7c01, 0x0030, 0x7de1, 0x1000, 0x0020, 0x7803, 0x1000, 0xc00d,
    0x7dc1, 0x001a, 0xa861, 0x7c01, 0x2000, 0x2161, 0x2000, 0x8463,
    0x806d, 0x7dc1, 0x000d, 0x9031, 0x7c10, 0x0018, 0x7dc1, 0x001a,
    0x9037, 0x61c1, 0x7dc1, 0x001a, 0x0000, 0x0000, 0x0000, 0x0000
  };
  
  disassemble (program, sizeof(program) / sizeof(program[0]));
  
  dcpu_t cpu = {0};
  run_with_debugger (program, sizeof(program) / sizeof(program[0]), &cpu);
  
  return 0;
}


