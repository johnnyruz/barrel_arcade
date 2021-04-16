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

            [Option('v', "verbose", Required = false, Default = false, HelpText = "Show binding process information and errors.")]
            public bool Verbose { get; set; }

            [Option('s', "set", Required = false, HelpText = "Set pedal bindings separated by commas", SetName = "Manual")]
            public string PedalBindings { get; set; }
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
                           if (!o.Default && !(o.PedalBindings?.Length > 0))
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
                               if (defaults == null || !(defaults.PedalBindings?.Length > 0))
                               {
                                   throw new Exception("No Default Values found in Defaults.json");
                               }

                               string ControlMessage = "#";
                               foreach (var b in defaults.PedalBindings)
                               {
                                   ControlMessage += $"{b.Trim().ToControlChar()}";
                               }

                               //Console.WriteLine($"DEBUG: Would Write Out: {ControlMessage}");
                               port.Open();
                               port.WriteLine(ControlMessage);
                               port.Close();
                           }
                           else //User has not supplied the --defaults command line argument, mapping command line values
                           {
                               var bindingArray = o.PedalBindings.Split(',');

                               string ControlMessage = "#";
                               foreach (var b in bindingArray)
                               {
                                   ControlMessage += $"{b.Trim().ToControlChar()}";
                               }

                               //Console.WriteLine($"DEBUG: Would Write Out: {ControlMessage}");
                               port.Open();
                               port.WriteLine(ControlMessage);
                               port.Close();

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
                h.Heading = "FrankenPedal v 0.2.0"; //change header
                h.AddPreOptionsLine("This application allows you to map keyboard " +
                    "and mouse inputs to any pedal using an Arduino and flashed with " +
                    "the FrankenPedal firmware.");
                h.AddPreOptionsLine("");
                h.AddPreOptionsLine("Reserved chars are:");
                h.AddPreOptionsLine("  - '~' or LB (Left Mouse Click)");
                h.AddPreOptionsLine("  - '!' or MB (Middle Mouse Click)");
                h.AddPreOptionsLine("  - '@' or RB (Right Mouse Click).");
                h.AddPreOptionsLine("\nExample - Set left to 'a', middle to left mouse, right to right mouse:\nFrankenPedal.exe -p COM4 -b 115200 -s a,LB,RB");
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
        public string[] PedalBindings { get; set; }
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
