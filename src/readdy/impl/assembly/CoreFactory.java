/*===========================================================================*\
*           ReaDDy - The Library for Reaction Diffusion Dynamics              *
* =========================================================================== *
* Copyright (c) 2010-2013, Johannes Schöneberg, Frank Noé, FU Berlin          *
* All rights reserved.                                                        *
*                                                                             *
* Redistribution and use in source and binary forms, with or without          *
* modification, are permitted provided that the following conditions are met: *
*                                                                             *
*     * Redistributions of source code must retain the above copyright        *
*       notice, this list of conditions and the following disclaimer.         *
*     * Redistributions in binary form must reproduce the above copyright     *
*       notice, this list of conditions and the following disclaimer in the   *
*       documentation and/or other materials provided with the distribution.  *
*     * Neither the name of Johannes Schöneberg or Frank Noé or the FU Berlin *
*       nor the names of its contributors may be used to endorse or promote   *
*       products derived from this software without specific prior written    *
*       permission.                                                           *
*                                                                             *
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" *
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE   *
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE  *
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE   *
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR         *
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF        *
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS    *
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN     *
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)     *
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE  *
* POSSIBILITY OF SUCH DAMAGE.                                                 *
*                                                                             *
\*===========================================================================*/
package readdy.impl.assembly;

import readdy.api.assembly.ICoreFactory;
import readdy.api.sim.core.ICore;
import readdy.api.sim.core.bd.IDiffusionEngine;
import readdy.api.sim.core.config.IParticleConfiguration;
import readdy.api.sim.core.rk.IReactionObserver;
import readdy.impl.sim.core.Core;

/**
 *
 * @author schoeneberg
 */
public class CoreFactory implements ICoreFactory {

    IParticleConfiguration particleConfig = null;
    IDiffusionEngine diffusionEngine = null;
    IReactionObserver reactionObserver = null;

    public void set_ParticleConfiguration(IParticleConfiguration particleConfig) {
        this.particleConfig = particleConfig;
    }

    public void set_DiffusionEngine(IDiffusionEngine diffusionEngine) {
        this.diffusionEngine = diffusionEngine;
    }

    public void set_ReactionObserver(IReactionObserver reactionObserver) {
        this.reactionObserver = reactionObserver;
    }

    public ICore createCore() {
        if (particleConfig != null
                && diffusionEngine != null
                && reactionObserver != null) {
            Core core = new Core();
            core.set_ParticleConfiguration(particleConfig);
            core.set_DiffusionEngine(diffusionEngine);
            core.set_ReactionObserver(reactionObserver);

            return core;
        } else {
            throw new RuntimeException("necessary building blocks not present for assembly."
                    + "construction cancelled!");
        }
    }
}
