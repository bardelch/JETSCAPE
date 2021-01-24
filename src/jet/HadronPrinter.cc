/***A****************************************************************************
 * Copyright (c) The JETSCAPE Collaboration, 2020
 *
 * Modular, task-based framework for simulating all aspects of heavy-ion collisions
 * 
 * For the list of contributors see AUTHORS.
 *
 * Report issues at https://github.com/JETSCAPE/JETSCAPE/issues
 *
 * or via email to bugs.jetscape@gmail.com
 *
 * Distributed under the GNU General Public License 3.0 (GPLv3 or later).
 * See COPYING for details.
 ******************************************************************************/

#include "HadronPrinter.h"
#include "JetScapeSignalManager.h"
namespace Jetscape {

// Register the module with the base class
RegisterJetScapeModule<HadronPrinter> HadronPrinter::reg("HadronPrinter");

HadronPrinter::HadronPrinter()
{
}

HadronPrinter::~HadronPrinter()
{
}

void HadronPrinter::Init()
{
  this->SetId("Printer");
  std::string OutputFileName = GetXMLElementText({"HadronPrinter", "OutputFileName"});
  fHadronOutfile.open(OutputFileName.c_str());

  KinematicVariableS = GetXMLElementText({"HadronPrinter", "KinematicVariable"});
  minD = GetXMLElementDouble({"HadronPrinter", "min"});
  maxD = GetXMLElementDouble({"HadronPrinter", "max"});
  
  if(KinematicVariableS.compare("y")==0){KinematicVariableInt=1;}
  if(KinematicVariableS.compare("eta")==0){KinematicVariableInt=2;}
  if(KinematicVariableS.compare("pt")==0){KinematicVariableInt=3;}
  if(KinematicVariableS.compare("e")==0){KinematicVariableInt=4;}
  if(KinematicVariableS.compare("phi")==0){KinematicVariableInt=5;}
  
  JSINFO<<"HadronPrinter variable is "<<KinematicVariableS<<", Int="<<KinematicVariableInt;
  
  JetScapeSignalManager::Instance()->ConnectGetFinalHadronListSignal(
      shared_from_this());

  //hadronPrinter->GetFinalHadronList.connect(hadro.get(), &Hadronization::GetHadrons);
}

void HadronPrinter::Exec()
{
  VERBOSE(2) <<"Run HadronPrinter: ";
  GetFinalHadronList(finalHadrons);
  PrintFinalHadron();
}

void HadronPrinter::Clear()
  {this->finalHadrons.clear();}

void HadronPrinter::WriteTask(weak_ptr<JetScapeWriter> w)
{
  VERBOSE(8);
  JetScapeTask::WriteTasks(w);
}

void HadronPrinter::PrintFinalHadron(){
  char buffer [33];
  //std:string y=1, eta=2, pt=3, e=4, phi=5;  //minD, maxD 
  JSINFO << "HadronPrinter : starting to print hadrons:";
  double y=0;
   
  fHadronOutfile << "#"<<KinematicVariableS<< endl;
  for(auto it : finalHadrons)
    {                                                                                                
      Hadron h = *it.get();      
      switch(KinematicVariableInt)
	{ 
	case 1:
	  y = 0.5*log( (h.e() + h.pz())/(h.e() - h.pz()) );
	  fHadronOutfile<<"  "<< y<<endl;
	  break;
	case 2:
	  fHadronOutfile<<"  "<< h.eta()<<endl;
	  break;
	case 3:
	  fHadronOutfile<<"  "<< h.pt()<<endl;
	  break;
	case 4: 
	  fHadronOutfile<<"  "<< h.e()<<endl;
	  break;   
	case 5: 
	  fHadronOutfile<<"  "<< h.phi()<<endl;
	  break;
	  
	default:
	  JSINFO << "The Kinematic variable isn’t valid";
	}
    }

  /*
   int i=0;
  fHadronOutfile << "#  PID  pstat  E  Px  Py  Pz  eta  phi" << endl;
  for(auto it : finalHadrons){
    Hadron h = *it.get();
    fHadronOutfile << i <<"  "<< h.pid() <<"  "<< h.pstat() <<"  "<< h.e() <<"  "<< h.px() <<"  "<< h.py() <<"  "<< h.pz() <<"  "<< h.eta() <<"  "<< h.phi() << endl;
    ++i;
  }
  */
}
} 


