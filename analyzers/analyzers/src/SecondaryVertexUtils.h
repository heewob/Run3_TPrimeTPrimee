#ifndef SECONDARY_VERTEX_UTILS_H
#define SECONDARY_VERTEX_UTILS_H

#include <vector>

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/Candidate/interface/VertexCompositePtrCandidate.h"
#include "RecoVertex/VertexTools/interface/VertexDistance3D.h"
#include "RecoVertex/VertexTools/interface/VertexDistanceXY.h"
#include "RecoVertex/VertexPrimitives/interface/ConvertToFromReco.h"
#include "RecoVertex/VertexPrimitives/interface/VertexState.h"

struct SVInfo {
  float chi2;
  float ndof;

  float dlen;
  float dlenSig;
  float dxy;
  float dxySig;

  float eta;
  float phi;
  float pt;
  float mass;

  float pAngle;

  float x;
  float y;
  float z;

  int   ntracks;
};

std::vector<SVInfo> extractSecondaryVertices(
    const std::vector<reco::VertexCompositePtrCandidate>& svs,
    const reco::Vertex& primaryVertex
);


#endif

