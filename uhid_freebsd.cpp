#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <unistd.h>

#include <vector>
#include <map>
#include <sstream>
#include <regex>

#include <Python.h>
#include <dev/usb/usb_ioctl.h>

PyDoc_STRVAR(uhid_freebsd_docs,
   "Get information on FreeBSD uhid devices\n");

PyDoc_STRVAR(enumerate_docs,
   "enumerate() -> list\n\n"
   "Returns the list of connected uhid devices\n");

PyDoc_STRVAR(get_report_data_docs,
   "get_report_data(fd, report_id) -> bytearray\n\n"
   "Returns the data portion of uhid report description\n");

static PyObject* enumerate(PyObject* self)
{
  auto devices =
    std::map<std::string, std::pair<unsigned int, unsigned int>>();

  if (auto dirp = opendir("/dev"))
  {
    dirent* dp;
    while ((dp = readdir(dirp)) != NULL)
    {
      auto devRegex = std::regex("^uhid([0-9]{0,3})$");
      std::cmatch cm;
      if (!std::regex_match(dp->d_name, cm, devRegex))
        continue;

      std::string name = "dev.uhid." + cm.str(1) + ".%pnpinfo";
      int i;
      int mib[4];
      size_t len;
      len = 4;
      if (sysctlnametomib(name.c_str(), mib, &len))
        continue;

      char buffer[1024];
      bzero(&buffer, sizeof(buffer));
      len = sizeof(buffer) - 1;

      if (sysctl(mib, 4, buffer, &len, NULL, 0) == -1)
        continue;

      unsigned int vendorId = 0;
      unsigned int productId = 0;

      auto vendorRegex = std::regex("(vendor=)(0x[a-f0-9]{4})");
      cm = std::cmatch{};
      if (!std::regex_search(buffer, cm, vendorRegex))
        continue;
      vendorId = std::stoul(cm.str(2), nullptr, 16);

      auto productRegex = std::regex("(product=)(0x[a-f0-9]{4})");
      if (!std::regex_search(buffer, cm, productRegex))
        continue;

      productId = std::stoul(cm.str(2), nullptr, 16);
      devices[dp->d_name] = {vendorId, productId};
    }
    closedir(dirp);
  }

  auto ret = PyList_New(0);
  for (const auto& device : devices)
  {
    auto dict = PyDict_New();
    auto deviceKey = Py_BuildValue("s", "device");
    auto deviceName = Py_BuildValue("s", device.first.c_str());
    PyDict_SetItem(dict, deviceKey, deviceName);
    auto pathKey = Py_BuildValue("s", "path");
    auto pathValue = Py_BuildValue("s",
      std::string("/dev/" + device.first).c_str());
    PyDict_SetItem(dict, pathKey, pathValue);
    auto vendorKey = Py_BuildValue("s", "vendor_id");
    auto vendorId = Py_BuildValue("l", device.second.first);
    PyDict_SetItem(dict, vendorKey, vendorId);
    auto productKey = Py_BuildValue("s", "product_id");
    auto productId = Py_BuildValue("l", device.second.second);
    PyDict_SetItem(dict, productKey, productId);
    PyList_Append(ret, dict);
  }
  return ret;
}

PyObject* get_report_data(PyObject *self, PyObject *args)
{
  int fd;
  int report_type;

  if (!PyArg_ParseTuple(args, "ii", &fd, &report_type))
      return NULL;

  char buffer[4096];
  bzero(buffer, sizeof(buffer));
  struct usb_gen_descriptor ugd;
  bzero(&ugd, sizeof(ugd));
  ugd.ugd_report_type = report_type;
  ugd.ugd_data = (void*) buffer;
  ugd.ugd_maxlen = sizeof(buffer);
  int ret = ioctl(fd, USB_GET_REPORT_DESC, &ugd);

  if (ret)
  {
    PyErr_SetString(PyExc_ValueError, "Couldn't get uhid report data");
    return NULL;
  }

  return PyByteArray_FromStringAndSize(
    static_cast<const char*>(ugd.ugd_data), ugd.ugd_actlen);
}

static PyMethodDef uhid_freebsd_funcs[] = {
#define ENTRY(name,flags) {#name, (PyCFunction)name, flags, name##_docs}
  ENTRY(enumerate, METH_VARARGS),
  ENTRY(get_report_data, METH_VARARGS),
#undef ENTRY
  {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC
#if PY_MAJOR_VERSION >= 3
PyInit_uhid_freebsd(void)
#else
inituhid_freebsd(void)
#endif
{
#if PY_MAJOR_VERSION >= 3
  static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "uhid_freebsd",         /* m_name */
    uhid_freebsd_docs,      /* m_doc */
    -1,                     /* m_size */
    uhid_freebsd_funcs,     /* m_methods */
    NULL,                   /* m_reload */
    NULL,                   /* m_traverse */
    NULL,                   /* m_clear */
    NULL                    /* m_free */
  };

  return PyModule_Create(&moduledef);
#else
  Py_InitModule3("uhid_freebsd", uhid_freebsd_funcs,
                 "Get information on FreeBSD uhid devices");
#endif
}
