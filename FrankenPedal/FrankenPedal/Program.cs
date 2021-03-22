using CommandLine;
using CommandLine.Text;
using System;
using System.IO;
using System.IO.Ports;
using System.Text.Json;

namespace FrankenPedal
{
    class Program
    {
        public class Options
        {

            [Option('p', "port", Required = false, HelpText = "Set the COM Port for the pedal to configure (i.e. COM4)")]
            public string ComPort { get; set; }

            [Option('b', "baud", Required = false, Default = 115200, HelpText = "Set the baud rate for the COM port.")]
            public int BaudRate { get; set; }

            [Option('d', "defaults", Required = false, HelpText = "Applies Default Bindings From Defaults.json file", SetName = "Default")]
            public bool Default { get; set; }

            [Option('l', "left", Required = false, HelpText = "Set the character binding for the left pedal", SetName = "Manual")]
            public string Left { get; set; }

            [Option('m', "middle", Required = false, HelpText = "Set the character binding for the middle pedal", SetName = "Manual")]
            public string Middle { get; set; }

            [Option('r', "right", Required = false, HelpText = "Set the character binding for the right pedal", SetName = "Manual")]
            public string Right { get; set; }

            [Option('v', "verbose", Required = false, Default = false, HelpText = "Show binding process information and errors.")]
            public bool Verbose { get; set; }

        }

        static void Main(string[] args)
        {

            var parser = new Parser(with => with.HelpWriter = null);
            var parserResult = parser.ParseArguments<Options>(args);
            parserResult
                   .WithParsed<Options>(o =>
                   {
                       try
                       {
                           if (!o.Default && !(o.Left?.Length > 0 || o.Middle?.Length > 0 || o.Right?.Length > 0))
                           {
                               throw new Exception("No Binding Arguments Provided! Run with --help to see usage information.");
                           }

                           SerialPort port = null;
                           PedalDefaults defaults = null;
                           if (File.Exists("Defaults.json"))  //Defaults.json successfully found
                           {
                               defaults = JsonSerializer.Deserialize<PedalDefaults>(File.ReadAllText("Defaults.json"));
                               if (o.ComPort?.Length > 0) //Deault port overridden by command like argument
                               {
                                   port = new SerialPort(o.ComPort, o.BaudRate);
                               }
                               else if (defaults.ComPort?.Length > 0) //Grab port + Baud Rate from Defaults.json file
                               {
                                   port = new SerialPort(defaults.ComPort, defaults.BaudRate);
                               }
                               else //COM port not specified in Defaults.json or Command Line Argument
                               {
                                   throw new Exception("Must specifiy COM port in command line or Defaults.json");
                               }
                           }
                           else //Defaults.json NOT found
                           {
                               if (o.ComPort?.Length > 0)
                               {
                                   port = new SerialPort(o.ComPort, o.BaudRate);
                               }
                               else
                               {
                                   throw new Exception("Must specifiy COM port in command line or Defaults.json");
                               }
                           }

                           if (o.Default) //User supplied the "defaults" command line argument
                           {
                               if (defaults == null || !(defaults.LeftPedal?.Length > 0 || defaults.MiddlePedal?.Length > 0 || defaults.RightPedal?.Length > 0))
                               {
                                   throw new Exception("No Default Values found in Defaults.json");
                               }

                               string ControlMessage = "";                               
                               if (defaults.LeftPedal?.Length > 0)
                               {
                                   ControlMessage += $"1{defaults.LeftPedal.ToControlChar()}";
                               }
                               if (defaults.MiddlePedal?.Length > 0)
                               {
                                   ControlMessage += $"2{defaults.MiddlePedal.ToControlChar()}";
                               }
                               if (defaults.RightPedal?.Length > 0)
                               {
                                   ControlMessage += $"3{defaults.RightPedal.ToControlChar()}";
                               }

                               port.Open();
                               port.WriteLine(ControlMessage);
                               port.Close();
                               //Console.WriteLine($"Would Write Out: {ControlMessage}");
                           }
                           else //User has not supplied the --defaults command line argument, mapping command line values
                           {
                               string ControlMessage = "";
                               if (o.Left != null)
                               {
                                   ControlMessage += $"1{o.Left.ToControlChar()}";
                               }
                               if (o.Middle != null)
                               {
                                   ControlMessage += $"2{o.Middle.ToControlChar()}";
                               }
                               if (o.Right != null)
                               {
                                   ControlMessage += $"3{o.Right.ToControlChar()}";
                               }
                               port.Open();
                               port.WriteLine(ControlMessage);
                               port.Close();
                               //Console.WriteLine($"Would Write Out: {ControlMessage}");                                 
                           }
                       }
                       catch (Exception e)
                       {
                           if (e.Source == "System.IO.Ports")
                           {
                               Console.WriteLine($"ERROR: {e.Message.Replace("file", "port")}");
                           }
                           else
                           {
                               Console.WriteLine($"ERROR: {e.Message}");
                           }
                           System.Environment.Exit(1);
                       }
                       Console.WriteLine("Binding Successful!");
                   })
                   .WithNotParsed(errs => DisplayHelp(parserResult));
        }

        static void DisplayHelp<T>(ParserResult<T> result)
        {
            var helpText = HelpText.AutoBuild(result, h =>
            {
                h.AdditionalNewLineAfterOption = false;
                h.Heading = "FrankenPedal v 0.1.0"; //change header
                h.AddPreOptionsLine("This application allows you to map keyboard " +
                    "and mouse inputs to the 3 pedals of an Infinity Pedal. This " +
                    "requires you to modify the pedal with an Arduino Leonardo microcontroller " +
                    "flashed with my firmware.");
                h.AddPreOptionsLine("");
                h.AddPreOptionsLine("Reserved chars are:");
                h.AddPreOptionsLine("  - '~' or LB (Left Mouse Click)");
                h.AddPreOptionsLine("  - '!' or MB (Middle Mouse Click)");
                h.AddPreOptionsLine("  - '@' or RB (Right Mouse Click).");
                h.AddPreOptionsLine("\nExample - Set left to 'a', middle to left mouse, right to right mouse:\nFrankenPedal.exe -p COM4 -b 9600 -l a -m LB -r '@'");
                h.AddPreOptionsLine("\nExample - Apply defaults from Defaults.json\nFrankenPedal.exe -p COM4 -d");
                return HelpText.DefaultParsingErrorsHandler(result, h);
            }, e => e);
            Console.WriteLine(helpText);
        }        
    }

    public class PedalDefaults
    {
        public string ComPort { get; set; }
        public int BaudRate { get; set; }
        public string LeftPedal { get; set; }
        public string MiddlePedal { get; set; }
        public string RightPedal { get; set; }
    }

    public static class ControlExtensions
    {
        public static char ToControlChar(this string input)
        {
            switch (input.ToUpperInvariant())
            {
                case "LB":
                    return '~';
                case "MB":
                    return '!';
                case "RB":
                    return '@';
                default:
                    return input.ToCharArray()[0];
            }
        }
    }
}
