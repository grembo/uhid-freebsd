from __future__ import print_function

import uhid_freebsd as uhid
import os

for dev in uhid.enumerate():
    print("Device:", dev["device"])
    print("  Path:", dev["path"])
    print("  VendorId: 0x%04x" % dev["vendor_id"])
    print("  ProductId: 0x%04x" % dev["product_id"])
    print("  ProductDesc:", dev["product_desc"])
    fd = os.open(dev["path"], os.O_RDONLY)
    out = uhid.get_report_data(fd, 3)
    os.close(fd)
    print("  ReportData: " +
          " ".join([("%02x" % (x)) for x in out]))
