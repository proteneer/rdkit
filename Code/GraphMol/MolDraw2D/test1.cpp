//
//  Copyright (C) 2015 Greg Landrum 
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKit.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKit source tree.
//

#include <RDGeneral/utils.h>
#include <RDGeneral/Invariant.h>
#include <RDGeneral/RDLog.h>
#include <GraphMol/RDKitBase.h>
#include <GraphMol/SmilesParse/SmilesParse.h>
#include <GraphMol/FileParsers/MolSupplier.h>
#include <GraphMol/FileParsers/FileParsers.h>
#include <GraphMol/Depictor/RDDepictor.h>
#include <GraphMol/FileParsers/MolFileStereochem.h>

#include <GraphMol/MolDraw2D/MolDraw2D.h>
#include <GraphMol/MolDraw2D/MolDraw2DSVG.h>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace RDKit;

void test1(){
  std::cout << " ----------------- Test 1" << std::endl;
  {
    std::string smiles="CO[C@@H](O)C1=C(O[C@H](F)Cl)C(C#N)=C1ONNC[NH3+]";
    ROMol *m = SmilesToMol(smiles);
    TEST_ASSERT(m);
    RDDepict::compute2DCoords(*m);
    WedgeMolBonds(*m,&(m->getConformer()));
    std::ofstream outs("test1_1.svg");
    MolDraw2DSVG drawer(300,300,outs);
    drawer.drawMolecule(*m);
    drawer.finishDrawing();
    outs.flush();
    delete m;
  }
  {
    // make sure this works with the stringstream too:
    std::string smiles="CO[C@@H](O)C1=C(O[C@H](F)Cl)C=C1ONNC[NH3+]";
    ROMol *m = SmilesToMol(smiles);
    TEST_ASSERT(m);
    RDDepict::compute2DCoords(*m);
    WedgeMolBonds(*m,&(m->getConformer()));
    MolDraw2DSVG drawer(300,300);
    drawer.drawMolecule(*m);
    drawer.finishDrawing();
    std::string text = drawer.getDrawingText();
    TEST_ASSERT(text.find("<svg:svg")!=std::string::npos);
    TEST_ASSERT(text.find("</svg:svg>")!=std::string::npos);
    delete m;
  }
  {
    std::string smiles="Cc1c(C(=O)NCCO)[n+](=O)c2ccccc2n1[O-]";
    ROMol *m = SmilesToMol(smiles);
    TEST_ASSERT(m);
    RDDepict::compute2DCoords(*m);
    WedgeMolBonds(*m,&(m->getConformer()));
    std::ofstream outs("test1_2.svg");
    MolDraw2DSVG drawer(300,300,outs);
    drawer.drawMolecule(*m);
    drawer.finishDrawing();
    outs.flush();
    delete m;
  }
  {
    std::string smiles="Cc1c(C(=O)NCCO)[n+](=O)c2ccccc2n1[O-]";
    ROMol *m = SmilesToMol(smiles);
    TEST_ASSERT(m);
    RDDepict::compute2DCoords(*m);
    WedgeMolBonds(*m,&(m->getConformer()));
    std::ofstream outs("test1_3.svg");
    MolDraw2DSVG drawer(300,300,outs);
    std::vector<int> highlights;
    highlights.push_back(0);
    highlights.push_back(4);
    highlights.push_back(5);
    drawer.drawMolecule(*m,&highlights);
    drawer.finishDrawing();
    outs.flush();
    delete m;
  }
  std::cout << " Done" << std::endl;
}

#ifdef RDK_CAIRO_BUILD
#include <cairo.h>
#include "MolDraw2DCairo.h"
void test2(){
  std::cout << " ----------------- Test 2" << std::endl;
  {
    std::string smiles="CO[C@@H](O)C1=C(O[C@H](F)Cl)C(C#N)=C1ONNC[NH3+]";
    ROMol *m = SmilesToMol(smiles);
    TEST_ASSERT(m);
    RDDepict::compute2DCoords(*m);
    WedgeMolBonds(*m,&(m->getConformer()));

    MolDraw2DCairo drawer(300,300);
    drawer.drawMolecule(*m);
    drawer.finishDrawing();
    
    drawer.writeDrawingText("test2_1.png");
    delete m;
  }
  {
    std::string smiles="Cc1c(C(=O)NCCO)[n+](=O)c2ccccc2n1[O-]";
    ROMol *m = SmilesToMol(smiles);
    TEST_ASSERT(m);
    RDDepict::compute2DCoords(*m);
    WedgeMolBonds(*m,&(m->getConformer()));

    MolDraw2DCairo drawer(300,300);
    drawer.drawMolecule(*m);
    drawer.finishDrawing();

    std::string drawing=drawer.getDrawingText();
    std::ofstream ofs("test2_2.png");
    ofs.write(drawing.c_str(),drawing.size());
    delete m;
  }
  {
    // ensure we still work with a client-provided drawing context
    std::string smiles="Cc1c(C(=O)NCCO)[n+](=O)c2ccccc2n1[O-]";
    ROMol *m = SmilesToMol(smiles);
    TEST_ASSERT(m);
    RDDepict::compute2DCoords(*m);
    WedgeMolBonds(*m,&(m->getConformer()));

    cairo_surface_t *surface =
      cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 300, 300);
    cairo_t *cr = cairo_create (surface);


    MolDraw2DCairo drawer(300,300,cr);
    std::vector<int> highlights;
    highlights.push_back(0);
    highlights.push_back(4);
    highlights.push_back(5);
    drawer.drawMolecule(*m,&highlights);
    drawer.finishDrawing();
    
    cairo_destroy (cr);
    cairo_surface_write_to_png (surface, "test2_3.png");
    cairo_surface_destroy (surface);

    delete m;
  }
  std::cout << " Done" << std::endl;
}
#else // RDK_CAIRO_BUILD
void test2(){
}
#endif


void test3(){
  std::cout << " ----------------- Test 3" << std::endl;
  {
    std::string smiles="C1CC1CC1ON1";
    std::string nameBase="test3_1";
    ROMol *m = SmilesToMol(smiles);
    TEST_ASSERT(m);
    RDDepict::compute2DCoords(*m);
    WedgeMolBonds(*m,&(m->getConformer()));
    static const int ha[] = {0,3,4,5};
    std::vector<int> highlight_atoms(ha, ha+sizeof(ha)/sizeof(int));
    std::map<int,std::string> atomLabels;
    atomLabels[2]="C1";
    atomLabels[1]="a<sub>3</sub><sup>4</sup>";
    atomLabels[0]="[CH2;X2:4]";
    atomLabels[6]="[NH2+:7]";
    
#ifdef RDK_CAIRO_BUILD
    {
      MolDraw2DCairo drawer(300,300);
      drawer.drawOptions().atomLabels = atomLabels;
      drawer.drawMolecule(*m,&highlight_atoms);
      drawer.finishDrawing();

      drawer.writeDrawingText(nameBase+".png");
    }
#endif    
    {
      std::ofstream outs((nameBase+".svg").c_str());
      MolDraw2DSVG drawer(300,300,outs);
      drawer.drawOptions().atomLabels = atomLabels;
      drawer.drawMolecule(*m,&highlight_atoms);
      drawer.finishDrawing();
      outs.flush();
    }
    delete m;
  }

  {
    std::string smiles="C1CC1CC1ON1";
    std::string nameBase="test3_2";
    ROMol *m = SmilesToMol(smiles);
    TEST_ASSERT(m);
    RDDepict::compute2DCoords(*m);
    WedgeMolBonds(*m,&(m->getConformer()));
    static const int ha[] = {0,3,4,5};
    std::vector<int> highlight_atoms(ha, ha+sizeof(ha)/sizeof(int));

#ifdef RDK_CAIRO_BUILD
    {
      MolDraw2DCairo drawer(300,300);
      drawer.drawOptions().circleAtoms=false;
      drawer.drawMolecule(*m,&highlight_atoms);
      drawer.finishDrawing();
      drawer.writeDrawingText(nameBase+".png");
    }
#endif    
    {
      std::ofstream outs((nameBase+".svg").c_str());
      MolDraw2DSVG drawer(300,300,outs);
      drawer.drawOptions().circleAtoms=false;
      drawer.drawMolecule(*m,&highlight_atoms);
      drawer.finishDrawing();
      outs.flush();
    }
    delete m;
  }
  {
    std::string smiles="Cc1c(C(=O)NCCO)[n+](=O)c2ccccc2n1[O-]";
    std::string nameBase="test3_3";
    ROMol *m = SmilesToMol(smiles);
    TEST_ASSERT(m);
    RDDepict::compute2DCoords(*m);
    WedgeMolBonds(*m,&(m->getConformer()));
    static const int ha[] = {11,12,13,14,15,16};
    std::vector<int> highlight_atoms(ha, ha+sizeof(ha)/sizeof(int));
    std::map<int,DrawColour> highlight_colors;
    highlight_colors[12]=DrawColour(0,0,1);
    highlight_colors[13]=DrawColour(0,1,0);
    
#ifdef RDK_CAIRO_BUILD
    {
      MolDraw2DCairo drawer(300,300);
      drawer.drawOptions().circleAtoms=true;
      drawer.drawMolecule(*m,&highlight_atoms,&highlight_colors);
      drawer.finishDrawing();
      drawer.writeDrawingText(nameBase+".png");
    }
#endif    
    {
      std::ofstream outs((nameBase+".svg").c_str());
      MolDraw2DSVG drawer(300,300,outs);
      drawer.drawOptions().circleAtoms=true;
      drawer.drawMolecule(*m,&highlight_atoms,&highlight_colors);
      drawer.finishDrawing();
      outs.flush();
    }
    delete m;
  }
  {
    std::string smiles="Cc1c(C(=O)NCCO)[n+](=O)c2ccccc2n1[O-]";
    std::string nameBase="test3_4";
    ROMol *m = SmilesToMol(smiles);
    TEST_ASSERT(m);
    RDDepict::compute2DCoords(*m);
    WedgeMolBonds(*m,&(m->getConformer()));
    static const int ha[] = {11,12,13,14,15,16,3};
    std::vector<int> highlight_atoms(ha, ha+sizeof(ha)/sizeof(int));
    std::map<int,DrawColour> highlight_colors;
    highlight_colors[12]=DrawColour(.5,.5,1);
    highlight_colors[13]=DrawColour(.5,1,.5);
    MolDrawOptions options;
    options.circleAtoms=true;
    options.highlightColour=DrawColour(1,.5,.5);
    options.continuousHighlight=true;
    
#ifdef RDK_CAIRO_BUILD
    {
      MolDraw2DCairo drawer(300,300);
      drawer.drawOptions() = options;
      drawer.drawMolecule(*m,&highlight_atoms,&highlight_colors);
      drawer.finishDrawing();
      drawer.writeDrawingText(nameBase+".png");
    }
#endif    
    {
      std::ofstream outs((nameBase+".svg").c_str());
      MolDraw2DSVG drawer(300,300,outs);
      drawer.drawOptions() = options;
      drawer.drawMolecule(*m,&highlight_atoms,&highlight_colors);
      drawer.finishDrawing();
      outs.flush();
    }
    delete m;
  }
  {
    std::string smiles="CCOC(=O)Nc1ccc(SCC2COC(Cn3ccnc3)(c3ccc(Cl)cc3Cl)O2)cc1";
    std::string nameBase="test3_5";
    ROMol *m = SmilesToMol(smiles);
    TEST_ASSERT(m);
    RDDepict::compute2DCoords(*m);
    WedgeMolBonds(*m,&(m->getConformer()));

    static const int ha[] = {17,18,19,20,21,6,7,8,9,31,32};
    std::vector<int> highlight_atoms(ha, ha+sizeof(ha)/sizeof(int));
    std::map<int,DrawColour> highlight_colors;
    MolDrawOptions options;
    options.circleAtoms=true;
    options.highlightColour=DrawColour(1,.5,.5);
    options.continuousHighlight=true;
    
#ifdef RDK_CAIRO_BUILD
    {
      MolDraw2DCairo drawer(200,200);
      drawer.drawOptions() = options;
      drawer.drawMolecule(*m,&highlight_atoms,&highlight_colors);
      drawer.finishDrawing();
      drawer.writeDrawingText(nameBase+".png");
    }
#endif    
    {
      std::ofstream outs((nameBase+".svg").c_str());
      MolDraw2DSVG drawer(200,200,outs);
      drawer.drawOptions() = options;
      drawer.drawMolecule(*m,&highlight_atoms,&highlight_colors);
      drawer.finishDrawing();
      outs.flush();
    }
    delete m;
  }

  {
    std::string smiles="CCOC(=O)Nc1ccc(SCC2COC(Cn3ccnc3)(c3ccc(Cl)cc3Cl)O2)cc1";
    std::string nameBase="test3_6";
    ROMol *m = SmilesToMol(smiles);
    TEST_ASSERT(m);
    RDDepict::compute2DCoords(*m);
    WedgeMolBonds(*m,&(m->getConformer()));

    MolDrawOptions options;
    static const int ha1[] = {17,18,19,20,21};
    std::vector<int> highlight_atoms1(ha1, ha1+sizeof(ha1)/sizeof(int));
    options.atomRegions.push_back(highlight_atoms1);
    static const int ha2[] = {6,7,8,9,31,32};
    std::vector<int> highlight_atoms2(ha2, ha2+sizeof(ha2)/sizeof(int));
    options.atomRegions.push_back(highlight_atoms2);
    options.includeAtomTags=true;

    
#ifdef RDK_CAIRO_BUILD
    {
      MolDraw2DCairo drawer(300,300);
      drawer.drawOptions() = options;
      drawer.drawMolecule(*m);
      drawer.finishDrawing();
      drawer.writeDrawingText(nameBase+".png");
    }
#endif    
    {
      std::ofstream outs((nameBase+".svg").c_str());
      MolDraw2DSVG drawer(300,300,outs);
      drawer.drawOptions() = options;
      drawer.drawMolecule(*m);
      drawer.finishDrawing();
      outs.flush();
    }
    delete m;
  }
  {
    std::string smiles="CCOC(=O)Nc1ccc(SCC2COC(Cn3ccnc3)(c3ccc(Cl)cc3Cl)O2)cc1";
    std::string nameBase="test3_7";
    ROMol *m = SmilesToMol(smiles);
    TEST_ASSERT(m);
    RDDepict::compute2DCoords(*m);
    WedgeMolBonds(*m,&(m->getConformer()));

    MolDrawOptions options;
    options.continuousHighlight=true;
    static const int ha[] = {17,20,25};
    std::vector<int> highlight_atoms(ha, ha+sizeof(ha)/sizeof(int));
    std::map<int,double> highlight_radii;
    highlight_radii[17]=0.5;
    highlight_radii[20]=1.0;
    std::map<int,DrawColour> highlight_colors;
    highlight_colors[17]=DrawColour(.5,.5,1.);
    
#ifdef RDK_CAIRO_BUILD
    {
      MolDraw2DCairo drawer(300,300);
      drawer.drawOptions() = options;
      drawer.drawMolecule(*m,&highlight_atoms,&highlight_colors,&highlight_radii);
      drawer.finishDrawing();
      drawer.writeDrawingText(nameBase+".png");
    }
#endif    
    {
      std::ofstream outs((nameBase+".svg").c_str());
      MolDraw2DSVG drawer(300,300,outs);
      drawer.drawOptions() = options;
      drawer.drawMolecule(*m,&highlight_atoms,&highlight_colors,&highlight_radii);
      drawer.finishDrawing();
      outs.flush();
    }
    delete m;
  }

  std::cout << " Done" << std::endl;
}

void test4(){
  std::cout << " ----------------- Test 4" << std::endl;
  {
    std::string fName = getenv("RDBASE");
    fName += "/Code/GraphMol/MolDraw2D/test_dir";
    fName += "/clash.mol";
    ROMol *m = MolFileToMol(fName);
    std::string nameBase="test4_1";
    TEST_ASSERT(m);
    
#ifdef RDK_CAIRO_BUILD
    {
      MolDraw2DCairo drawer(300,300);
      drawer.drawMolecule(*m);
      drawer.finishDrawing();
      drawer.writeDrawingText(nameBase+".png");
    }
#endif    
    {
      std::ofstream outs((nameBase+".svg").c_str());
      MolDraw2DSVG drawer(300,300,outs);
      drawer.drawMolecule(*m);
      drawer.finishDrawing();
      outs.flush();
    }
    delete m;
  }

  std::cout << " Done" << std::endl;
}



void test5(){
  std::cout << " ----------------- Test 5" << std::endl;
  {
    std::string smiles="*c1cc(*)cc(*)c1";
    std::string nameBase="test5_1";
    ROMol *m = SmilesToMol(smiles);
    TEST_ASSERT(m);
    RDDepict::compute2DCoords(*m);
    WedgeMolBonds(*m,&(m->getConformer()));
    MolDrawOptions options;
    options.dummiesAreAttachments=true;
    options.atomLabels[0]="R1";
#ifdef RDK_CAIRO_BUILD
    {
      MolDraw2DCairo drawer(300,300);
      drawer.drawOptions() = options;
      drawer.drawMolecule(*m);
      drawer.finishDrawing();
      drawer.writeDrawingText(nameBase+".png");
    }
#endif    
    {
      std::ofstream outs((nameBase+".svg").c_str());
      MolDraw2DSVG drawer(300,300,outs);
      drawer.drawOptions() = options;
      drawer.drawMolecule(*m);
      drawer.finishDrawing();
      outs.flush();
    }
    delete m;
  }
  std::cout << " Done" << std::endl;
}



#ifdef RDK_TEST_MULTITHREADED
#include <boost/thread.hpp>  
namespace {
  void runblock(const std::vector<ROMol *> &mols,const std::vector<std::string> &refData,
                unsigned int count,unsigned int idx){
    for(unsigned int j=0;j<200;j++){
      for(unsigned int i=0;i<mols.size();++i){
        if(i%count != idx) continue;
        ROMol *mol = mols[i];
        MolDraw2DSVG drawer(300,300);
        drawer.drawMolecule(*mol);
        drawer.finishDrawing();
        std::string text = drawer.getDrawingText();
        TEST_ASSERT(text==refData[i]);
      }
    }
  }
}

void testMultiThreaded(){
  std::cout << " ----------------- Test multi-threaded drawing" << std::endl;
  std::string fName = getenv("RDBASE");
  fName += "/Data/NCI/first_200.props.sdf";
  RDKit::SDMolSupplier suppl(fName);
  std::cerr<<"reading molecules"<<std::endl;
  std::vector<ROMol *> mols;
  while(!suppl.atEnd()&&mols.size()<100){
    ROMol *mol=0;
    try{
      mol=suppl.next();
    } catch(...){
      continue;
    }
    if(!mol) continue;
    mols.push_back(mol);
  }

  std::cerr<<"generating reference drawings"<<std::endl;
  std::vector<std::string> refData(mols.size());
  for(unsigned int i=0;i<mols.size();++i){
    MolDraw2DSVG drawer(300,300);
    drawer.drawMolecule(*(mols[i]));
    drawer.finishDrawing();
    refData[i]=drawer.getDrawingText();
    TEST_ASSERT(refData[i].find("<svg:svg")!=std::string::npos);
    TEST_ASSERT(refData[i].find("</svg:svg>")!=std::string::npos);
  }

  boost::thread_group tg;
  unsigned int count=4;
  std::cerr<<"processing"<<std::endl;
  for(unsigned int i=0;i<count;++i){
    std::cerr<<" launch :"<<i<<std::endl;std::cerr.flush();
    tg.add_thread(new boost::thread(runblock,mols,refData,count,i));
  }
  tg.join_all();
  std::cerr<<" Done"<<std::endl;
}
#else
void testMultiThreaded(){
}
#endif

int main(){
  RDLog::InitLogs();
  test1();
  test2();
  test3();
  test4();
  test5();
  testMultiThreaded();
}
