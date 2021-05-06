==============
 uhid-freebsd
==============

uhid-freebsd is a Python library to retrieve information on USB HID devices
on FreeBSD (`/dev/uhid*`).


Installation
------------

.. code-block:: bash

   pkg install py36-uhid-freebsd

or

.. code-block:: bash

   pkg install py27-uhid-freebsd

or

.. code-block:: bash

   cd /usr/ports/devel/py-uid-freebsd
   make install clean


Usage
-----

.. code-block:: python

   from __future__ import print_function

   import uhid_freebsd as uhid
   import os

   for dev in uhid.enumerate():
       print("Device:", dev["device"])
       print("  Path:", dev["path"])
       print("  VendorId: 0x%04x" % dev["vendor_id"])
       print("  ProductId: 0x%04x" % dev["product_id"])
       print("  ProductDesc:", dev["product_desc"])
       print("  SerialNumber:", dev["serial_number"])
       fd = os.open(dev["path"], os.O_RDONLY)
       out = uhid.get_report_data(fd, 3)
       os.close(fd)
       print("  ReportData: " +
             " ".join([("%02x" % (x)) for x in out]))


Contributing
------------

Pull requests are welcome.  For major changes, please open an issue first to
discuss what you would like to change.

Please make sure to update tests as appropriate.

License
-------
`BSD 2-Clause <https://choosealicense.com/licenses/bsd-2-clause/>`__
