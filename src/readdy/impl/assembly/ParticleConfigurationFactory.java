// ==========================================================================
//           ReaDDy - The Library for Reaction Diffusion Dynamics
// ==========================================================================
// Copyright (c) 2010-2013, Johannes Schöneberg, Frank Noé, FU Berlin
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of Johannes Schöneberg or Frank Noé or the FU Berlin
//       nor the names of its contributors may be used to endorse or promote
//       products derived from this software without specific prior written
//       permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// ==========================================================================

package readdy.impl.assembly;

import readdy.api.assembly.IParticleConfigurationFactory;
import readdy.api.io.in.par_global.IGlobalParameters;
import readdy.api.io.in.tpl_coord.ITplgyCoordinatesFileData;
import readdy.api.sim.core.config.IParticleConfiguration;
import readdy.api.sim.core.particle.IParticleParameters;
import readdy.api.sim.core.space.INeighborSearch;
import readdy.impl.sim.core.config.ParticleConfiguration;
import readdy.impl.sim.core.space.NeighborLattice;

/**
 *
 * @author schoeneberg
 */
public class ParticleConfigurationFactory implements IParticleConfigurationFactory{
    ITplgyCoordinatesFileData tplgyCoordsData = null;
    IParticleParameters particleParameters = null;
    IGlobalParameters globalParameters = null;

    public void set_tplgyCoordinatesFileData(ITplgyCoordinatesFileData tplgyCoordsData) {
        this.tplgyCoordsData = tplgyCoordsData;
    }

    public void set_particleParameters(IParticleParameters particleParameters){
        this.particleParameters = particleParameters;
    }

    public void set_globalParameters(IGlobalParameters globalParameters){
        this.globalParameters = globalParameters;
    }

    public IParticleConfiguration createParticleConfiguration() {
        if(settedUpProperly()){
            ParticleConfiguration particleConfig= new ParticleConfiguration();

            ParticleFactory particleFactory = new ParticleFactory();
            particleFactory.set_particleParameters(particleParameters);
            particleConfig.setParticleFactory(particleFactory);
            particleConfig.set_ParticleParameters(particleParameters);
            
            particleConfig.setupInitialParticleConfiguration(tplgyCoordsData.get_CoordinatesFileDataList());
            // neighborList and particleConfig know each other!
            double[][] latticeBounds = globalParameters.get_latticeBounds();
            double latticeBoxSize = globalParameters.get_latticeBoxSize();
            

            INeighborSearch neighborSearch = new NeighborLattice(latticeBounds,latticeBoxSize ,
                                                            particleConfig,
                                                            particleParameters);

            particleConfig.setNeighborSearch(neighborSearch);


            return particleConfig;
        }else{
            throw new RuntimeException("ParticleConfigurationFactory not setted up properly");
        }
    }

    private boolean settedUpProperly() {
        return (tplgyCoordsData != null
                && particleParameters != null
                && globalParameters != null);
    }

  



}
