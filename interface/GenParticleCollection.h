
#pragma once

#include <vector>
#include <algorithm>
#include <memory>

#include <TTreeReader.h>
#include <TTreeReaderArray.h>

#include "Math/Vector4D.h"
#include "Math/Vector4Dfwd.h"

#include "mut_framework/mut_dataformats/interface/Candidate.h"

  typedef ROOT::Math::PtEtaPhiMVector PtEtaPhiMVector;

  class GenParticleCollection : public std::vector<mut::Candidate> {

    public:

      // TTreeReaderValues for reading from VHBB_HEPPY
      // had to use raw pointers because TTreeReaderArray is badly implemented
      TTreeReaderArray<float> * gen_particle_pts_;
      TTreeReaderArray<float> * gen_particle_etas_;
      TTreeReaderArray<float> * gen_particle_phis_;
      TTreeReaderArray<float> * gen_particle_masss_;

      GenParticleCollection() :
        gen_particle_pts_(nullptr),
        gen_particle_etas_(nullptr),
        gen_particle_phis_(nullptr),
        gen_particle_masss_(nullptr)
      {
      }
      
      GenParticleCollection(TTreeReader & reader, std::string obj_name, bool load = false)
    {
      if (load) { 
        gen_particle_pts_   = new TTreeReaderArray<float>(reader, (obj_name+"_pt").c_str());
        gen_particle_etas_  = new TTreeReaderArray<float>(reader, (obj_name+"_eta").c_str());
        gen_particle_phis_  = new TTreeReaderArray<float>(reader, (obj_name+"_phi").c_str());
        gen_particle_masss_ = new TTreeReaderArray<float>(reader, (obj_name+"_mass").c_str());
      } else {
        gen_particle_pts_   = nullptr;
        gen_particle_etas_  = nullptr;
        gen_particle_phis_  = nullptr;
        gen_particle_masss_ = nullptr;
      }
    } 

    ~GenParticleCollection() {
      delete gen_particle_pts_; 
      delete gen_particle_etas_; 
      delete gen_particle_phis_; 
      delete gen_particle_masss_; 
    }
    
    void update() {

      if (gen_particle_pts_ != nullptr) {

        // delete previous elements
        this->clear();

        // use default order
        std::vector<int> order(gen_particle_pts_->GetSize());
        std::iota(order.begin(), order.end(), 0); 

        // iterate over particles in order 
        for (const auto & i : order) {
          
          PtEtaPhiMVector gen_particle_lv((*gen_particle_pts_)[i],
                                          (*gen_particle_etas_)[i],
                                          (*gen_particle_phis_)[i],
                                          (*gen_particle_masss_)[i]); 
          // new element using constructor from PtEtaPhiM
        	this->emplace_back(gen_particle_lv);
          
        }
      }
    }
    
};

