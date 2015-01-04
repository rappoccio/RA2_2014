#ifndef Config_H
#define Config_H
#include "vector"
#include "string"
#include "fstream"
#include "map"
#include <iostream>
 #include "TH2F.h"
#include "stdlib.h"
#include "algorithm"
#include "TString.h"
// self written stuff
#include "SharedVariables.h"
class Efficiency2D
{
public:
  Efficiency2D();
  Efficiency2D(std::string NameInFile, std::string variableX, std::string variableY, std::string label){NameInFile_=NameInFile;variableX_=variableX;variableY_=variableY;label_=label;initilized_=false;initilizedY_=false;initilizedX_=false; TH2Loaded_=false;}
  std::string GetNameInFile(){return NameInFile_;}
  std::string GetLabel(){return label_;}
  std::string GetXVariable(){return variableX_;}
  std::string GetYVariable(){return variableY_;}
  void SetXVectorPosition(unsigned int IntOrFloat, unsigned int Position){XIntOrFloat_=IntOrFloat; XPosition_=Position;initilizedX_=true;if(initilizedY_ && TH2Loaded_)initilized_=true;}
  void SetYVectorPosition(unsigned int IntOrFloat, unsigned int Position){YIntOrFloat_=IntOrFloat; YPosition_=Position;initilizedY_=true;if(initilizedX_ && TH2Loaded_)initilized_=true;}
  bool GetInitializedStatuts(){return initilized_;}
  void SetTH2F(TH2F *th2f){th2f_=th2f; TH2Loaded_=true; if(initilizedX_ && initilizedY_)initilized_=true;}
  TH2F* GetTH2F(){return th2f_;}
  ~Efficiency2D(){};
private:
  std::string NameInFile_, variableX_, variableY_, label_;
  unsigned int XIntOrFloat_, XPosition_;
  unsigned int YIntOrFloat_, YPosition_;
  TH2F *th2f_;
  bool initilizedX_, initilizedY_, initilized_, TH2Loaded_;
};

class Efficiency1D
{
public:
  Efficiency1D();
  Efficiency1D(std::string NameInFile, std::string variableX, std::string label){NameInFile_=NameInFile;variableX_=variableX;label_=label;initilized_=false; initilizedX_=false;TH1Loaded_=false;}
  std::string GetNameInFile(){return NameInFile_;}
  std::string GetLabel(){return label_;}
  std::string GetXVariable(){return variableX_;}
  void SetXVectorPosition(unsigned int IntOrFloat, unsigned int Position){XIntOrFloat_=IntOrFloat; XPosition_=Position;initilizedX_=true; if(TH1Loaded_)initilized_=true;}
  void SetTH1F(TH1F *th1f){th1f_=th1f;TH1Loaded_=true; if(initilizedX_)initilized_=true;}
  bool GetInitializedStatuts(){return initilized_;}
  TH1F* GetTH1F(){return th1f_;}
  ~Efficiency1D(){}; 
private:
  std::string NameInFile_, variableX_,  label_;
  unsigned int XIntOrFloat_, XPosition_;
  bool initilized_, initilizedX_,TH1Loaded_;
  TH1F * th1f_;
};
class CutI
{
public:
  CutI();
  CutI(std::string Variable, std::string CutTypStr, unsigned int CutTyp, int CutValue){Variable_=Variable;CutTypStr_=CutTypStr;CutTyp_=CutTyp; CutValue_=CutValue;initilized_=false;}
  std::string GetVariable(){return Variable_;}
  void SetVectorPosition(unsigned int ii){vectorPosition_ = ii; initilized_=true;}
  std::string GetCutTypStr(){return CutTypStr_;}
  unsigned int GetCutTyp(){return CutTyp_;}
  int GetCutValueI(){return CutValue_;}
  ~CutI(){}
private:
  std::string Variable_;
  std::string CutTypStr_;
  unsigned int CutTyp_;
  int CutValue_;
  bool initilized_;
  unsigned int vectorPosition_;
};
class CutF
{
public:
  CutF();
  CutF(std::string Variable, std::string CutTypStr, unsigned int CutTyp, float CutValue){Variable_=Variable;CutTypStr_=CutTypStr;CutTyp_=CutTyp; CutValue_=CutValue;initilized_=false;}
  std::string GetVariable(){return Variable_;}
  std::string GetCutTypStr(){return CutTypStr_;}
  void SetVectorPosition(unsigned int ii){vectorPosition_ = ii; initilized_=true;}
  unsigned int GetCutTyp(){return CutTyp_;}
  float GetCutValueF(){return CutValue_;}
  ~CutF(){}
private:
  std::string Variable_;
  std::string CutTypStr_;
  unsigned int CutTyp_;
  float CutValue_;
  bool initilized_;
  unsigned int vectorPosition_;
};
class Prediction
{
public:
  Prediction();
  Prediction(std::string Name, std::vector<CutI*> cutVectorI,std::vector<CutF*> cutVectorF, std::vector<std::string> PurityEfficiencies,std::vector<std::string> ControlSampleEfficiencies, std::vector<std::string> ScalingControlSampleToPredictionSample, std::vector<std::string> PredictionEfficiencies)
  {
    Name_=Name;
    cutVectorI_=cutVectorI;
    cutVectorF_=cutVectorF;
    PurityEfficiencies_=PurityEfficiencies;
    ControlSampleEfficiencies_=ControlSampleEfficiencies;
    ScalingControlSampleToPredictionSample_=ScalingControlSampleToPredictionSample;
    PredictionEfficiencies_=PredictionEfficiencies;
  }
  std::string GetName(){return Name_;}
  std::vector<CutI*> GetCutVectorI(){return cutVectorI_;}
  std::vector<CutF*> GetCutVectorF(){return cutVectorF_;}
  bool Initialize();
  void DumpStatus();
  Float_t GetPurityScaling(std::vector<Float_t> treeVariablesFloat_, std::vector<Int_t>  treeVariablesInt_, Float_t weight);
  ~Prediction(){};
private:
  std::string Name_;
  std::vector<std::string> PurityEfficiencies_, ControlSampleEfficiencies_, ScalingControlSampleToPredictionSample_, PredictionEfficiencies_;
  std::vector<CutI*> cutVectorI_;
  std::vector<CutF*> cutVectorF_;

};
class Cuts
{
public:
  Cuts();
  Cuts(std::string VariableName, std::string cutTyp,unsigned int CutTyp, double value){VariableName_=VariableName;cutTyp_=cutTyp;CutTyp_=CutTyp;value_=value;isUShort_=false;}
  Cuts(std::string VariableName, std::string cutTyp,unsigned int CutTyp, unsigned int value,bool ushort){VariableName_=VariableName;cutTyp_=cutTyp;CutTyp_=CutTyp;valueI_=value;isUShort_=ushort;}
  std::string VariableName(){VariableName_;}
  std::string VariableTyp()
  {
    unsigned int position = VariableName_.find(";");
    if(position>VariableName_.size())return "EROOOOROROORORORO";
    std::string result = VariableName_.substr(position+1);
    return result;
    
  };
  std::string cutTyp(){return cutTyp_;}
  unsigned int CutTyp(){return CutTyp_;}
  double value(){return value_;}
  unsigned int valueI(){return valueI_;}
  bool isInt(){return isUShort_;}
  ~Cuts(){}
private:
  std::string VariableName_, cutTyp_;
  double value_;
  unsigned int CutTyp_, valueI_;
  bool isUShort_;
};
class Variable
{
public:
  Variable();
  Variable(std::string Name, std::string TypString){Name_=Name; SetUp(TypString);}
  std::string GetVariable() const {return Name_;}
  std::string GetTypString() const {return TypString_;}
  std::string GetLabel() const {return label_;}
  ~Variable();
private:
  std::string Name_, TypString_, label_;
  void SetUp(std::string TypString);
};
class Config
{
public:
  Config();
  Config(const char* ConfFile);
  std::string GetName(){return fileName_;}
  std::string GetInputRootTreeName(){return InputRootTreeName_;}
  std::vector<std::string> GetInputRootFileName(){return InputRootFileNameVector_;}
  std::pair <std::string,std::string> SeparateString(std::string InputStr, std::string Separater);
  std::vector<std::string> SeparateStringToVector(std::string InputStr, std::string Separater);
  std::map<std::string,Variable*> GetVariableMap(){return variablesMap_;}
  std::vector<CutI*> GetCutIVector(){return cutVectorI_;}
  std::vector<CutF*> GetCutFVector(){return cutVectorF_;}
  std::string GetEfficiencyFileName(){return EfficiencyFileName_;}
  std::string GetEfficiencyFolderName(){return EfficiencyFolderName_;}
  std::map<std::string,Efficiency2D*> Get2DEffMap(){return efficiencies2D_;}
  std::map<std::string,Efficiency1D*> Get1DEffMap(){return efficiencies1D_;};
  std::map<std::string,Prediction*> GetPredictionsMap(){return predictions_;}
  ~Config();
private:
  std::string fileName_;
  void ReadFile();
  std::ifstream fileStr_;
  // used operators for analysization 
  std::vector<std::string> operators_;
  // configuration stuff
  std::vector<std::string> InputRootFileNameVector_;
  std::string InputRootTreeName_;
  std::map<std::string,Variable*> variablesMap_;
  std::vector<CutI*> cutVectorI_;
  std::vector<CutF*> cutVectorF_;
  std::string EfficiencyFileName_, EfficiencyFolderName_;
  std::map<std::string,Efficiency2D*> efficiencies2D_;
  std::map<std::string,Efficiency1D*> efficiencies1D_;
  std::map<std::string,Prediction*> predictions_;
};


#endif