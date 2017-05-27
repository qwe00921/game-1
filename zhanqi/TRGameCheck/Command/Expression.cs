using Microsoft.CSharp;
using System;
using System.CodeDom.Compiler;
using System.Collections.Generic;
using System.Reflection;
using System.Text;

namespace TRGameCheck.Command
{
    public class Expression
    {
        private object instance;
        private MethodInfo method;
        /// <summary>
        /// 表达试运算
        /// </summary>
        /// <param name="expression">表达试</param>
        public Expression(string expression)
        {
            string className = "Expression";
            string methodName = "ComputeInt";

            CompilerParameters p = new CompilerParameters();
            p.GenerateInMemory = true;

            string text = string.Format("using System;sealed class {0} {{public string {1}(int x){{return {2};}}}}",
                className, methodName, expression);
            CompilerResults cr = new CSharpCodeProvider().CompileAssemblyFromSource(p, text);
            if (cr.Errors.Count > 0)
            {
                string msg = "Expression(\"" + expression + "\"): \n";
                foreach (CompilerError err in cr.Errors)
                {
                    msg += err.ToString() + "\n";
                }
                throw new ApplicationException(msg);
            }
            instance = cr.CompiledAssembly.CreateInstance(className);
            method = instance.GetType().GetMethod(methodName);
        }

        /// <summary>
        /// 处理数据
        /// </summary>
        /// <param name="x"></param>
        /// <returns>返回计算值</returns>
        public string ComputeInt(int x)
        {
            return (string)method.Invoke(instance, new object[] { x });
        }
    }
}
