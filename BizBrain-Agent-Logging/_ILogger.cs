using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace BizBrainCommonUtils
{
    [ ComVisible(true),
  Guid("FC24FC02-FCB3-4644-87CD-73A368138A61"),
  InterfaceType(ComInterfaceType.InterfaceIsDual)
 ]
    public interface _ILogger
    {
        void Log(object message);
      
    }
}
