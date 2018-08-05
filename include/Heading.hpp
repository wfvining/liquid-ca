#ifndef _HEADING_HPP
#define _HEADING_HPP

#include <iostream>

class Heading
{
private:
   double _heading_radians;
public:
   Heading(double h);
   Heading();
   ~Heading();

   /**
    * Return the heading in radians.
    */
   double Radians() const;

   friend bool    operator== (const Heading& h1, const Heading& h2);
   friend bool    operator!= (const Heading& h1, const Heading& h2);
   friend Heading operator-  (const Heading& h1, const Heading& h2);
   friend Heading operator+  (const Heading& h1, const Heading& h2);
   friend std::ostream& operator<< (std::ostream& out, const Heading& h);
};

#endif // _HEADING_HPP
