/*********************************************************************
 *
 * Copyright (C) 2007,  Simon Kagstrom
 *
 * Filename:      elf.hh
 * Author:        Simon Kagstrom <simon.kagstrom@gmail.com>
 * Description:   ELF stuff
 *
 * $Id:$
 *
 ********************************************************************/
#ifndef __ELF_HH__
#define __ELF_HH__

#include <stdint.h>
#include <stdlib.h>

#include <libelf.h>
#include <ght_hash_table.h>

class CibylElf;

class ElfSymbol
{
public:
  ElfSymbol(int index, int binding, uint32_t addr, uint32_t size, int type, const char *name)
  {
    this->index = index;
    this->binding = binding;
    this->addr = addr;
    this->size = size;
    this->type = type;
    this->name = name;
  }

  int index;
  int binding;
  uint32_t addr;
  uint32_t size;
  int type;
  const char *name;
};


class ElfReloc
{
public:
  ElfReloc(uint32_t addr, int type, ElfSymbol *sym)
  {
    this->addr = addr;
    this->type = type;
    this->sym = sym;
  }

  uint32_t addr;
  int type;
  ElfSymbol *sym;
};

class ElfSection
{
public:
  ElfSection(const char *name, uint8_t *data, size_t size,
             int type, uint32_t align)
  {
    this->name = name;
    this->data = data;
    this->size = size;
    this->type = type;
    this->align = align;
  }

  uint8_t *data;
  size_t   size;
  int      type;
  uint32_t align;
  const char *name;
};

/* Found in the special .cibylexpsyms function */
typedef struct
{
  uint32_t name;
  uint32_t addr;
} cibyl_exported_symbol_t;

class CibylElf
{
public:
  static CibylElf *getInstance();

  CibylElf(const char *filname);

  ElfSymbol **getSymbolsByType(char type);

  ElfSymbol **getFunctions();

  int getNumberOfFunctions();

  ElfReloc **getRelocations();

  int getNumberOfRelocations();

  uint32_t getEntryPoint() { return this->entryPoint; }

  const char *getCibylStrtabString(uint32_t offset)
  {
    /* This is safe since we assert that this section is there in
     * the constructor */
    return ((const char*)this->getSection(".cibylstrtab")->data) + offset;
  }

  ElfSection *getSection(const char *name)
  {
    return (ElfSection*)ght_get(this->sections_by_name,
                                strlen(name), name);
  }

  ElfReloc *getRelocationBySymbol(ElfSymbol *sym);

private:
  void addSection(ElfSection *section);

  void handleSymtab(Elf_Scn *scn);

  void fixupSymbolSize(ElfSymbol **table, int n, uint32_t sectionEnd);

  static CibylElf *instance;

  Elf *elf; /* from libelf */

  ElfSymbol **symbols;
  ElfSymbol **functionSymbols;
  ElfSymbol **dataSymbols;

  int n_symbols;
  int n_functionSymbols;
  int n_dataSymbols;
  ght_hash_table_t *symtable;

  ght_hash_table_t *sections_by_name;
  ght_hash_table_t *relocations_by_symbol;

  ElfReloc **relocs;
  int n_relocs;

  uint32_t entryPoint;
};

extern CibylElf *elf;

#endif /* !__ELF_HH__ */
