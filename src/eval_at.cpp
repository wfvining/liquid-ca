#include "Model.hpp"
#include "LCAFactory.hpp"

LCAFactory factory;

int num_iterations;

int main(int argc, char** argv)
{
   LCAFactory factory;

   int arg_index = factory.Init(argc, argv);
   double initial_density = atof(argv[arg_index]);
   int num_correct = 0;

   for(int i = 0; i < 100; i++)
   {
      std::unique_ptr<LCA> lca = factory.Create(initial_density);
      lca->MinimizeMemory();
      lca->Run([](const ModelStats& s) {
                  return (s.CurrentCADensity() == 0.0 || s.CurrentCADensity() == 1.0);
               });

      if(lca->GetStats().IsCorrect())
      {
         num_correct++;
      }
   }

   std::cout << initial_density << " " << num_correct << std::endl;
}
