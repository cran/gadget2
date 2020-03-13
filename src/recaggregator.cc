#include "recaggregator.h"
#include "areatime.h"
#include "readfunc.h"
#include "stock.h"
#include "stockprey.h"
#include "poppredator.h"
#include "fleet.h"
#include "readword.h"
#include "errorhandler.h"
#include "gadget.h"
#include "global.h"

RecAggregator::RecAggregator(const FleetPtrVector& Fleets,
  const StockPtrVector& Stocks, LengthGroupDivision* const Lgrpdiv,
  const IntMatrix& Areas, const IntMatrix& Ages, Tags* tag)
  : taggingExp(tag), LgrpDiv(Lgrpdiv), areas(Areas), ages(Ages),
    doescatch(Fleets.Size(), Stocks.Size(), 0), suitptr(0), alptr(0) {

  int i, j;
  //JMB its simpler to just store pointers to the predators
  //and preys rather than pointers to the fleets and stocks
  for (i = 0; i < Stocks.Size(); i++)
    preys.resize(Stocks[i]->getPrey());
  for (i = 0; i < Fleets.Size(); i++)
    predators.resize(Fleets[i]->getPredator());

  for (i = 0; i < preys.Size(); i++) {
    CI.resize(new ConversionIndex(preys[i]->getLengthGroupDiv(), LgrpDiv));
    if (CI[i]->Error())
      handle.logMessage(LOGFAIL, "Error in recaggregator - error when checking length structure");
  }

  for (i = 0; i < predators.Size(); i++)
    for (j = 0; j < preys.Size(); j++)
      if (predators[i]->doesEat(preys[j]->getName()))
        doescatch[i][j] = 1;

  //Resize total using dummy variables tmppop and popmatrix.
  PopInfo tmppop;
  tmppop.N = 1.0;
  PopInfoMatrix popmatrix(ages.Nrow(), LgrpDiv->numLengthGroups(), tmppop);
  total.resize(areas.Nrow(), 0, 0, popmatrix);
  this->Reset();
}

RecAggregator::~RecAggregator() {
  int i;
  for (i = 0; i < CI.Size(); i++)
    delete CI[i];
}

void RecAggregator::Print(ofstream& outfile) const {
  int i, j;
  for (i = 0; i < total.Size(); i++) {
    outfile << "\tInternal areas";
    for (j = 0; j < areas.Ncol(i); j++)
      outfile << sep << areas[i][j];
    outfile << endl;
    total[i].printNumbers(outfile);
  }
  outfile.flush();
}

void RecAggregator::Reset() {
  int i;
  for (i = 0; i < total.Size(); i++)
    total[i].setToZero();
}

void RecAggregator::Sum() {

  int f, g, h, i, j, k, r;
  int predl = 0;  //JMB there is only ever one length group ...
  double ratio;

  this->Reset();
  for (f = 0; f < predators.Size(); f++) {
    for (h = 0; h < preys.Size(); h++) {
      if (doescatch[f][h]) {
        for (r = 0; r < areas.Nrow(); r++) {
          for (j = 0; j < areas.Ncol(r); j++) {
            if ((preys[h]->isPreyArea(areas[r][j])) && (predators[f]->isInArea(areas[r][j]))) {
              for (i = 0; i < predators[f]->numPreys(); i++) {
                if (strcasecmp(preys[h]->getName(), predators[f]->getPrey(i)->getName()) == 0) {

                  //JMB removed the overconsumption stuff
                  suitptr = &predators[f]->getSuitability(i)[predl];
                  alptr = &taggingExp->getConsumptionALK(areas[r][j], preys[h]->getName());
                  ratio = predators[f]->getConsumptionRatio(areas[r][j], i, predl);
                  for (g = 0; g < ages.Nrow(); g++)
                    for (k = 0; k < ages.Ncol(g); k++)
                      if ((alptr->minAge() <= ages[g][k]) && (ages[g][k] <= alptr->maxAge()))
                        total[r][g].Add((*alptr)[ages[g][k]], *CI[h], *suitptr, ratio);
                }
              }
            }
          }
        }
      }
    }
  }
}
