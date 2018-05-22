// Variant 2 (about the weather)
#include <iostream>
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <libxml++-2.6/libxml++/libxml++.h>
#include <libxml++-2.6/libxml++/parsers/textreader.h>
#include <fstream>
using namespace std;

struct indent
{
   int depth_;
   indent(int depth): depth_(depth) {};
};

std::ostream & operator<<(std::ostream & o, indent const & in) //function for style
{
   for(int i = 0; i != in.depth_; ++i)
  {
      o << "   ";
  }
   return o;
}

int main(int argc, char* argv[])
{
   string cityname, cityname_checker; //cityname_checker is for finding our city
   string filepath; //name of the file
   if(argc > 1)
      filepath = argv[1];
   else
      cin >> filepath; 

   if(argc > 2)
      cityname = argv[2];
   else
      cin >> cityname;

   cityname_checker = cityname;
   try
   {
      xmlpp::TextReader reader(filepath);

      while(reader.read())
      {
         int depth = reader.get_depth();
         if(depth == 1 && reader.get_name() == "city") //looking for our city
         {
            if(reader.has_attributes())
             {
                reader.move_to_first_attribute();
                do
                {
                   if(reader.get_name() == "name")
                   cityname_checker = reader.get_value();
                }
                while(reader.move_to_next_attribute());
             }
            if(cityname_checker != cityname) //checking if we've got the right city
                continue; // if not we start looking for another city
            else
                cout << "INFORMATION FOR CITY OF " << cityname << ":\n";
            do
              {
                string name = reader.get_name();
                if(name != "#text" && name != "current" && name != "country") // these names bring bugs to the programme, that's why I check for them
                {
                  std::cout << indent(depth) << reader.get_name() << ":\n"; // printing weather information
                  if(reader.has_attributes())
                  {
                    reader.move_to_first_attribute();
                    do
                    {
                      std::cout << indent(depth) << "    " << reader.get_name() << ": " << reader.get_value() << "\n";
                    }
                    while(reader.move_to_next_attribute());
                    reader.move_to_element();
                  }
                  if(reader.has_value())
                    std::cout << indent(depth) << "   value: " << reader.get_value() << std::endl;
                }
              }
            while(reader.read());
         }
      }
   }
   catch(const std::exception& e)
   {
        std::cout << "Exception caught: " << e.what() << std::endl;
   }
}
