// -*- C++ -*-
//
// Package:    METDouble
// Class:      METDouble
// 
/**\class METDouble METDouble.cc RA2Classic/METDouble/src/METDouble.cc
 * 
 * Description: [one line class summary]
 * 
 * Implementation:
 *     [Notes on implementation]
 */
//
// Original Author:  Arne-Rasmus Draeger,68/111,4719,
//         Created:  Fri Apr 11 16:35:33 CEST 2014
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
//#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrectorCalculator.h"

//
// class declaration
//

class METDouble : public edm::EDProducer {
public:
	explicit METDouble(const edm::ParameterSet&);
	~METDouble();
	
	static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
	
private:
	virtual void beginJob() ;
	virtual void produce(edm::Event&, const edm::EventSetup&);
	virtual void endJob() ;
	
	virtual void beginRun(edm::Run&, edm::EventSetup const&);
	virtual void endRun(edm::Run&, edm::EventSetup const&);
	virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
	virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
	edm::InputTag metTag_;
  edm::InputTag RhoTag_;
	edm::InputTag MuTag_;
	edm::InputTag JetTag_;
  //  std::vector<std::string> jecPayloadNames_;
  bool corrMet;
	  double corrEx;
	  double corrEy;
	  double corrSumEt;
  std::string l1file;
  std::string l2file;
  std::string l3file;
	
	// ----------member data ---------------------------
};

//
// constants, enums and typedefs
//


//
// static data member definitions
//

//
// constructors and destructor
//
METDouble::METDouble(const edm::ParameterSet& iConfig)
  //  jecPayloadNames_( iConfig.getParameter<std::vector<std::string> >("jecPayloadNames") ) // JEC level payloads
{
	//register your produc
	metTag_ = iConfig.getParameter<edm::InputTag> ("METTag");
	RhoTag_ = iConfig.getParameter<edm::InputTag> ("RhoTag");
	MuTag_ = iConfig.getParameter<edm::InputTag> ("MuTag");
	JetTag_ = iConfig.getParameter<edm::InputTag> ("JetTag");
	corrMet = iConfig.getParameter<bool> ("corrMet");
	l1file = iConfig.getParameter<std::string> ("L1File");
	l2file = iConfig.getParameter<std::string> ("L2File");
	l3file = iConfig.getParameter<std::string> ("L3File");
	
	produces<double>("Pt");
	produces<double>("Phi");
	produces<double>("PtRaw");
	produces<double>("PhiRaw");
	/* Examples
	 *   produces<ExampleData2>();
	 * 
	 *   //if do put with a label
	 *   produces<ExampleData2>("label");
	 * 
	 *   //if you want to put into the Run
	 *   produces<ExampleData2,InRun>();
	 */
	//now do what ever other initialization is needed
	
}


METDouble::~METDouble()
{
	
	// do anything here that needs to be done at desctruction time
	// (e.g. close files, deallocate resources etc.)
	
}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
METDouble::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	double metpt_=0, metphi_=0;
	double rawmetpt_=0, rawmetphi_=0;
	edm::Handle< edm::View<pat::MET> > MET;
	iEvent.getByLabel(metTag_,MET); 
	edm::Handle<double> rho_ ;
	iEvent.getByLabel(RhoTag_, rho_);
	edm::Handle< edm::View<pat::Jet> > Jets;
        iEvent.getByLabel(JetTag_,Jets);
	edm::Handle< edm::View<pat::Muon> > Muons;
        iEvent.getByLabel(MuTag_,Muons);

	//  Load the JetCorrectorParameter objects into a vector, IMPORTANT: THE ORDER MATTERS HERE !!!! 
	std::vector<JetCorrectorParameters> vPar;
	/*	for ( std::vector<std::string>::const_iterator payloadBegin = jecPayloadNames_.begin(),
		payloadEnd = jecPayloadNames_.end(), ipayload = payloadBegin; ipayload != payloadEnd; ++ipayload ) {
	  JetCorrectorParameters pars(*ipayload);
	  vPar.push_back(pars);
	  }*/

	std::vector<JetCorrectorParameters> vParL1;
	//	vParL1.push_back(JetCorrectorParameters(jecPayloadNames_[0]));
	
	JetCorrectorParameters *L3JetPar  = new JetCorrectorParameters(l3file);
	JetCorrectorParameters *L2JetPar  = new JetCorrectorParameters(l2file);
	JetCorrectorParameters *L1JetPar  = new JetCorrectorParameters(l1file);

	vPar.push_back(*L1JetPar);
	vPar.push_back(*L2JetPar);
	vPar.push_back(*L3JetPar);
	vParL1.push_back(*L1JetPar);

	FactorizedJetCorrector *JetCorrector = new FactorizedJetCorrector(vPar);
	FactorizedJetCorrector *JetCorrectorL1 = new FactorizedJetCorrector(vParL1);
	
	if (corrMet) {
	  corrEx = 0;
	  corrEy = 0;
	  corrSumEt = 0;

	  bool skipEM_ = true;
	  double skipEMfractionThreshold_ = 0.9;
	  bool skipMuons_ = true;
	  //double jetCorrEtaMax_ = 9.9;
	  double type1JetPtThreshold_ = 10.0;

	  edm::View<pat::Jet>::const_iterator ijet = Jets->begin()-1;
	  for (const pat::Jet &jet : *Jets) {

	    ijet++;

	    reco::Candidate::LorentzVector uncorrJet;
	    // The pat::Jet "knows" if it has been corrected, so here
	    // we can "uncorrect" the entire jet to apply the corrections
	    // we want here.
	    pat::Jet const * pJet = dynamic_cast<pat::Jet const *>( &*ijet );
	    if ( pJet != 0 ) {
	      uncorrJet = pJet->correctedP4(0);
	    }
	    // Otherwise, if we do not have pat::Jets on input, we just assume
	    // the user has not corrected them upstream and use it as raw.
	    else {
	      uncorrJet = ijet->p4();
	    }
	    
	    JetCorrector->setJetEta(uncorrJet.eta());
	    JetCorrector->setJetPt(uncorrJet.pt());
	    JetCorrector->setJetA(ijet->jetArea());
	    JetCorrector->setRho(*(rho_.product())); 

	    double corr = JetCorrector->getCorrection();
	    
	    double emEnergyFraction = jet.chargedEmEnergyFraction() + jet.neutralEmEnergyFraction();
	    if ( skipEM_ && emEnergyFraction > skipEMfractionThreshold_ ) continue;
	    
	    reco::Candidate::LorentzVector rawJetP4 = jet.correctedP4(0);
	    
	    if ( skipMuons_ && jet.muonMultiplicity() != 0 ) {
	      for (const pat::Muon &muon : *Muons) {
		if( !muon.isGlobalMuon() && !muon.isStandAloneMuon() ) continue;
		TLorentzVector muonV; muonV.SetPtEtaPhiE(muon.p4().pt(),muon.p4().eta(),muon.p4().phi(),muon.p4().e());
		TLorentzVector jetV; jetV.SetPtEtaPhiE(jet.p4().pt(),jet.p4().eta(),jet.p4().phi(),jet.p4().e());
		if( muonV.DeltaR(jetV) < 0.5 ){
		  reco::Candidate::LorentzVector muonP4 = muon.p4();
		  rawJetP4 -= muonP4;
		}
	      }
	    }

	    reco::Candidate::LorentzVector corrJetP4 = corr*rawJetP4;

	    if ( corrJetP4.pt() > type1JetPtThreshold_ ) {
	      reco::Candidate::LorentzVector tmpP4 = jet.correctedP4(0);
	      JetCorrectorL1->setJetEta(tmpP4.eta());
	      JetCorrectorL1->setJetPt(tmpP4.pt());
	      JetCorrectorL1->setJetA(ijet->jetArea());
	      JetCorrectorL1->setRho(*(rho_.product())); 
	    
	      corr = JetCorrectorL1->getCorrection();
	      reco::Candidate::LorentzVector rawJetP4offsetCorr = corr*rawJetP4;
	      corrEx -= (corrJetP4.px() - rawJetP4offsetCorr.px());
	      corrEy -= (corrJetP4.py() - rawJetP4offsetCorr.py());
	      corrSumEt += (corrJetP4.Et() - rawJetP4offsetCorr.Et());
	    }
	    
	  }
	  //	  pat::MET pmet = pat::MET( &*(MET->at(0)) );

	  const float rawPt = MET->at(0).shiftedPt(pat::MET::NoShift, pat::MET::Raw);
	  const float rawPhi = MET->at(0).shiftedPhi(pat::MET::NoShift, pat::MET::Raw);
	  //	  const float rawSumEt = MET->at(0).shiftedSumEt(pat::MET::NoShift, pat::MET::Raw);
	  TVector2 rawMET_;
	  rawMET_.SetMagPhi (rawPt, rawPhi );
	  Double_t rawPx = rawMET_.Px();
	  Double_t rawPy = rawMET_.Py();
	  //	  Double_t rawEt = std::hypot(rawPx,rawPy);

	  double pxcorr = rawPx+corrEx;
	  double pycorr = rawPy+corrEy;
	  double et = std::hypot(pxcorr,pycorr);
	  //	  double sumEtcorr = rawSumEt+corrSumEt;
	  TLorentzVector corrmet; corrmet.SetPxPyPzE(pxcorr,pycorr,0.,et);

	  metpt_= et;
	  metphi_ = corrmet.Phi();
	  rawmetpt_ = rawPt;
	  rawmetphi_ = rawPhi;

	  //	  std::cout<<"corrEx: "<<corrEx<<" rawPx: "<<rawPx<<" raw met: "<<rawPt<<" new met: "<<et<<std::endl;
	  //metcorrPx_ = corrEx;
	  //metCorrPy_ = corrEy;
	  //metSumEt_ = sumEtcorr;
	}

	else {
	  if(MET.isValid() ){
	    metpt_=MET->at(0).pt();
	    metphi_=MET->at(0).phi();
	    rawmetpt_ = MET->at(0).pt();
	    rawmetphi_ = MET->at(0).phi();
	  }	
	  else std::cout<<"METDouble::Invlide Tag: "<<metTag_.label()<<std::endl;
	}

	delete JetCorrector;
	delete JetCorrectorL1;
	delete L1JetPar;
	delete L2JetPar;
	delete L3JetPar;

	std::auto_ptr<double> htp(new double(metpt_));
	iEvent.put(htp,"Pt");
	std::auto_ptr<double> htp2(new double(metphi_));
	iEvent.put(htp2,"Phi");
	std::auto_ptr<double> htp3(new double(rawmetpt_));
	iEvent.put(htp3,"PtRaw");
	std::auto_ptr<double> htp4(new double(rawmetphi_));
	iEvent.put(htp4,"PhiRaw");
	
}

// ------------ method called once each job just before starting event loop  ------------
void 
METDouble::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
METDouble::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void 
METDouble::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
METDouble::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
METDouble::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
METDouble::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
METDouble::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
	//The following says we do not know what parameters are allowed so do no validation
	// Please change this to state exactly what you do use, even if it is no parameters
	edm::ParameterSetDescription desc;
	desc.setUnknown();
	descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(METDouble);
