using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using log4net;
using log4net.Appender;
using log4net.Core;
using log4net.Repository;

namespace BizBrainCommonUtils
{
    [Serializable]
    public class Logger : _ILoggerEx
    {
        [NonSerialized]
        protected log4net.ILog log;
        protected Level loggingLevel = Level.Debug;
        protected Level defaultLevel = Level.All;
        public Level DefaultLevel { get => defaultLevel; set => defaultLevel = value; }
        protected bool isDebugEnabled, isInfoEnabled, isWarnEnabled, isErrorEnabled, isFatalEnabled;
        protected string name;
        protected Type type;
        static Logger()
        {
            log4net.Config.XmlConfigurator.Configure();
        }
        public bool IsDebugEnabled
        {
            get => isDebugEnabled;
            set
            {

                isDebugEnabled = value;
                isInfoEnabled = value;
                isWarnEnabled = value;
                isErrorEnabled = value;
                isFatalEnabled = value;

            }
        }

        public bool IsInfoEnabled
        {
            get => isInfoEnabled;

            set
            {


                isInfoEnabled = value;
                isWarnEnabled = value;
                isErrorEnabled = value;
                isFatalEnabled = value;

            }

        }
        public bool IsWarnEnabled
        {
            get => isWarnEnabled;

            set
            {

                isWarnEnabled = value;
                isErrorEnabled = value;
                isFatalEnabled = value;

            }

        }
        public bool IsErrorEnabled
        {
            get => isErrorEnabled;

            set
            {

                isErrorEnabled = value;
                isFatalEnabled = value;

            }

        }
        public bool IsFatalEnabled
        {
            get => isFatalEnabled; set
            {
                isFatalEnabled = value;
            }
        }
        public Level LoggingLevel
        {
            get => loggingLevel; set
            {
                loggingLevel = value;
                if (loggingLevel == Level.Debug)
                {
                    IsDebugEnabled = true;
                }
                else if (loggingLevel == Level.Info)
                {
                    IsInfoEnabled = true;

                }
                else if (loggingLevel == Level.Warn)
                {
                    IsWarnEnabled = true;
                }
                else if (loggingLevel == Level.Error)
                {
                    IsErrorEnabled = true;
                }
                else if (loggingLevel == Level.Fatal)
                {
                    IsFatalEnabled = true;
                }
            }
        }

        protected void Initialize()
        {
            isDebugEnabled = true;
            isInfoEnabled = true;
            isWarnEnabled = true;
            isErrorEnabled = true;
            isFatalEnabled = true;
        }
        
        public Logger() {
            Initialize();
            log= log4net.LogManager.GetLogger(System.Reflection.MethodBase.GetCurrentMethod().DeclaringType);
        }
        public Logger(string name)
        {
            Initialize();
            this.name = name;
            log = log4net.LogManager.GetLogger(name);
        }
        public Logger(Type type)
        {
            Initialize();
            this.type = type;
            log = log4net.LogManager.GetLogger(type);
        }


        public virtual void Debug(object message)
        {

            Log(Level.Debug, message);
        }


        public virtual void Error(object message)
        {
            Log(Level.Error, message);
        }

        public virtual void Fatal(object message)
        {
            Log(Level.Fatal, message);
        }

        public virtual void Info(object message)
        {
            Log(Level.Info, message);
        }
        public virtual void Warn(object message)
        {
            Log(Level.Warn, message);
        }
        public virtual void Log(Level level, object message)
        {
            if (log == null)
            {
                if (!string.IsNullOrEmpty(name))
                {
                    log = log4net.LogManager.GetLogger(name);
                }
                else if (type != null)
                {
                    log = log4net.LogManager.GetLogger(type);
                }
                else
                {
                    log = log4net.LogManager.GetLogger(System.Reflection.MethodBase.GetCurrentMethod().DeclaringType);
                }
            }
            if (level == Level.All)
            {
                log.Debug(message);
            }
            else if (level == Level.Debug && IsDebugEnabled)
            {
                log.Debug(message);
            }
            else if (level == Level.Info && IsInfoEnabled)
            {
                log.Info(message);
            }
            else if (level == Level.Warn && IsWarnEnabled)
            {
                log.Warn(message);
            }
            else if (level == Level.Error && IsErrorEnabled)
            {
                log.Error(message);
            }
            else if (level == Level.Fatal && IsFatalEnabled)
            {
                log.Fatal(message);
            }
            FlushBuffers();
        }

        public virtual void Log(object message)
        {
            Log(defaultLevel, message);
            FlushBuffers();
        }
        public void FlushBuffers()
        {
            ILoggerRepository rep = LogManager.GetRepository();
            foreach (IAppender appender in rep.GetAppenders())
            {
                var buffered = appender as BufferingAppenderSkeleton;
                if (buffered != null)
                {
                    buffered.Flush();
                }


            }
        }
    }
}