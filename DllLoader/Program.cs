using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DllLoader
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.Write("Injecting dll...");
            try
            {
                DllInjector.Inject("BattlefrontII", "RconServer.dll");
                Console.WriteLine("OK");
            }
            catch (Exception e)
            {
                Console.WriteLine("Failed.");
                Console.WriteLine(e.ToString());
            }

            Console.WriteLine("Press [return] to exit.");
            Console.ReadLine();
        }
    }
}