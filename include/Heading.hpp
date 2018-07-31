#ifndef _HEADING_HPP
#define _HEADING_HPP

class Heading
{
private:
   double _heading_radians;
public:
   Heading(double h);
   ~Heading();

   /**
    * Return the heading in radians.
    */
   double Radians() const;

   friend bool    operator== (const Heading& h1, const Heading& h2);
   friend bool    operator!= (const Heading& h1, const Heading& h2);
   friend Heading operator-  (const Heading& h1, const Heading& h2);
   friend Heading operator+  (const Heading& h1, const Heading& h2);
};

#endif // _HEADING_HPP
