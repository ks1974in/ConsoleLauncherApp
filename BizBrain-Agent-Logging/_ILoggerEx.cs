using log4net.Core;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BizBrainCommonUtils
{
    public interface _ILoggerEx:_ILogger
    {
        void Log(Level level, object message);
        void Debug(object message);
        void Info(object message);
        void Warn(object message);
        void Error(object message);
        void Fatal(object message);
        bool IsDebugEnabled { get; set; }
        bool IsInfoEnabled { get; set; }
        bool IsWarnEnabled { get; set; }
        bool IsErrorEnabled { get; set; }
        bool IsFatalEnabled { get; set; }
        Level DefaultLevel { get; set; }
        Level LoggingLevel { get; set; }
      
        
    }
}
