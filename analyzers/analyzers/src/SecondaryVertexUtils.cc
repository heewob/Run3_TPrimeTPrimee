#include "SecondaryVertexUtils.h"
#include <cmath>

std::vector<SVInfo> extractSecondaryVertices(
    const std::vector<reco::VertexCompositePtrCandidate>& svs,
    const reco::Vertex& pv
) {
  std::vector<SVInfo> out;

  VertexDistance3D vdist;
  VertexDistanceXY vdistXY;

  for (const auto& sv : svs) {

    SVInfo info;

    info.chi2 = sv.vertexChi2();
    info.ndof = sv.vertexNdof();

    // 3D decay length
    Measurement1D dl = vdist.distance(
      pv,
      VertexState(
        RecoVertex::convertPos(sv.position()),
        RecoVertex::convertError(sv.error())
      )
    );
    // not putting the cut `if (dl.value() > dlenMin_ and dl.significance() > dlenSigMin_)` because we want to keep all the information
    info.dlen    = dl.value();
    info.dlenSig = dl.significance();

    // 2D decay length
    Measurement1D d2d = vdistXY.distance(
      pv,
      VertexState(
        RecoVertex::convertPos(sv.position()),
        RecoVertex::convertError(sv.error())
      )
    );
    info.dxy    = d2d.value();
    info.dxySig = d2d.significance();

    // Kinematics
    info.pt   = sv.pt();
    info.eta  = sv.eta();
    info.phi  = sv.phi();
    info.mass = sv.mass();

    // Position
    info.x = sv.vx();
    info.y = sv.vy();
    info.z = sv.vz();

    // Tracks
    info.ntracks = sv.numberOfDaughters();

    // Pointing angle
    double dx = sv.vx() - pv.x();
    double dy = sv.vy() - pv.y();
    double dz = sv.vz() - pv.z();

    double dot = dx*sv.px() + dy*sv.py() + dz*sv.pz();
    double magv = std::sqrt(dx*dx + dy*dy + dz*dz);
    double magp = sv.p();

    info.pAngle = (magv > 0 && magp > 0) ? std::acos(dot / (magv * magp)) : -1.0;

    out.push_back(info);
  }

  return out;
}


