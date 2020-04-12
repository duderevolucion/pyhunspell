/*  hunspell.c
 *
 *  Copyright (C) 2009 - Sayamindu Dasgupta <sayamindu@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Library General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <hunspell.h>


/****************************************
                HunSpell
****************************************/


typedef struct {
	PyObject_HEAD 
    Hunhandle *handle;
} HunSpell;


static PyObject *
HunSpell_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
  HunSpell *self;
  self = (HunSpell *) type->tp_alloc(type,0);
  return (PyObject *) self;
}

static int
HunSpell_init(HunSpell * self, PyObject *args, PyObject *kwds)
{
	char *dpath;
	char *apath;

	if (!PyArg_ParseTuple(args, "ss", &dpath, &apath))
		return 1;

	self->handle = Hunspell_create(apath, dpath);
	return 0;
}

static void
HunSpell_dealloc(HunSpell * self)
{
	Hunspell_destroy(self->handle);
	Py_TYPE(self)->tp_free((PyObject *) self);
}

static PyObject *
HunSpell_get_dic_encoding(HunSpell * self, PyObject *args)
{
	return Py_BuildValue("s", Hunspell_get_dic_encoding(self->handle));
}

static PyObject *
HunSpell_spell(HunSpell * self, PyObject *args)
{
	char *word;
	int retvalue;

	if (!PyArg_ParseTuple(args, "s", &word))
		return NULL;
	retvalue = Hunspell_spell(self->handle, word);

	return PyBool_FromLong(retvalue);
}


static PyObject *
HunSpell_suggest(HunSpell * self, PyObject *args)
{
	char *word, **slist;
	int i, num_slist;
	PyObject *slist_list;

	if (!PyArg_ParseTuple(args, "s", &word))
		return NULL;

	slist_list = PyList_New(0);
	num_slist = Hunspell_suggest(self->handle, &slist, word);

	for (i = 0; i < num_slist; i++) {
		PyList_Append(slist_list, Py_BuildValue("s", slist[i]));
	}

	Hunspell_free_list(self->handle, &slist, num_slist);
	return slist_list;
}

static PyObject *
HunSpell_analyze(HunSpell * self, PyObject *args)
{
	char *word, **slist;
	int i, num_slist;
	PyObject *slist_list;

	if (!PyArg_ParseTuple(args, "s", &word))
		return NULL;

	slist_list = PyList_New(0);
	num_slist = Hunspell_analyze(self->handle, &slist, word);

	for (i = 0; i < num_slist; i++) {
		PyList_Append(slist_list, Py_BuildValue("s", slist[i]));
	}

	Hunspell_free_list(self->handle, &slist, num_slist);
	return slist_list;
}

static PyObject *
HunSpell_stem(HunSpell * self, PyObject *args)
{
	char *word, **slist;
	int i, num_slist;
	PyObject *slist_list;

	if (!PyArg_ParseTuple(args, "s", &word))
		return NULL;

	slist_list = PyList_New(0);
	num_slist = Hunspell_stem(self->handle, &slist, word);

	for (i = 0; i < num_slist; i++) {
		PyList_Append(slist_list, Py_BuildValue("s", slist[i]));
	}

	Hunspell_free_list(self->handle, &slist, num_slist);
	return slist_list;
}

static PyObject *
HunSpell_generate(HunSpell * self, PyObject *args)
{
	char *word1, *word2, **slist;
	int i, num_slist;
	PyObject *slist_list;

	if (!PyArg_ParseTuple(args, "ss", &word1, &word2))
		return NULL;

	slist_list = PyList_New(0);
	num_slist = Hunspell_generate(self->handle, &slist, word1, word2);

	for (i = 0; i < num_slist; i++) {
		PyList_Append(slist_list, Py_BuildValue("s", slist[i]));
	}

	Hunspell_free_list(self->handle, &slist, num_slist);
	return slist_list;
}

static PyObject *
HunSpell_add(HunSpell * self, PyObject *args)
{
	char *word;
	int retvalue;

	if (!PyArg_ParseTuple(args, "s", &word))
		return NULL;
	retvalue = Hunspell_add(self->handle, word);

	return Py_BuildValue("i", retvalue);
}

static PyObject *
HunSpell_add_with_affix(HunSpell * self, PyObject *args)
{
	char *word, *example;
	int retvalue;

	if (!PyArg_ParseTuple(args, "ss", &word, &example))
		return NULL;
	retvalue = Hunspell_add_with_affix(self->handle, word, example);

	return Py_BuildValue("i", retvalue);
}

static PyObject *
HunSpell_remove(HunSpell * self, PyObject *args)
{
	char *word;
	int retvalue;

	if (!PyArg_ParseTuple(args, "s", &word))
		return NULL;
	retvalue = Hunspell_remove(self->handle, word);

	return Py_BuildValue("i", retvalue);
}

static PyMethodDef HunSpell_methods[] = {
	{"get_dic_encoding", (PyCFunction) HunSpell_get_dic_encoding,
	 METH_NOARGS,
	 "Gets encoding of loaded dictionary."},
	{"spell", (PyCFunction) HunSpell_spell, METH_VARARGS,
	 "Checks the spelling of the given word."},
	{"suggest", (PyCFunction) HunSpell_suggest, METH_VARARGS,
	 "Provide suggestions for the given word."},
	{"analyze", (PyCFunction) HunSpell_analyze, METH_VARARGS,
	 "Provide morphological analysis for the given word."},
	{"stem", (PyCFunction) HunSpell_stem, METH_VARARGS,
	 "Stemmer method."},
	{"generate", (PyCFunction) HunSpell_generate, METH_VARARGS,
	 "Provide morphological generation for the given word."},
	{"add", (PyCFunction) HunSpell_add, METH_VARARGS,
	 "Adds the given word into the runtime dictionary"},
	{"add_with_affix", (PyCFunction) HunSpell_add_with_affix, METH_VARARGS,
	 "Adds the given word with affix flags of the example (a dictionary word) \
     into the runtime dictionary"},
	{"remove", (PyCFunction) HunSpell_remove, METH_VARARGS,
	 "Removes the given word from the runtime dictionary"},
	{NULL,NULL,0,NULL}
};

static PyTypeObject HunSpellType = {
  PyVarObject_HEAD_INIT(NULL, 0)
  .tp_name = "Hunspell",
  .tp_dealloc = (destructor) HunSpell_dealloc,
  .tp_basicsize = sizeof(HunSpell),
  .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
  .tp_doc = "HunSpell object",
  .tp_methods = HunSpell_methods,
  .tp_init = (initproc) HunSpell_init,
  .tp_new = (initproc) HunSpell_new,
};


static PyModuleDef hunspellmodule = {
  PyModuleDef_HEAD_INIT,
  .m_name = "hunspell",
  .m_doc = "Hunspell",
  .m_size = -1,
};


/******************** Module Initialization function ****************/

PyMODINIT_FUNC
PyInit_hunspell(void)
{
	PyObject *mod;

	if (PyType_Ready(&HunSpellType) < 0 )
	  return NULL;

	mod = PyModule_Create( &hunspellmodule );
	if (mod==NULL)
	  return NULL;

	Py_INCREF(&HunSpellType);
	if (PyModule_AddObject(mod, "Hunspell", (PyObject *) &HunSpellType) < 0) {
	  Py_DECREF(&HunSpellType);
	  Py_DECREF(mod);
	  return NULL;
	}

	return mod;
}
