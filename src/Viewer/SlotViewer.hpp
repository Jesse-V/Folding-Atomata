
/******************************************************************************\
                     This file is part of Folding Atomata,
          a program that displays 3D views of Folding@home proteins.

                      Copyright (c) 2013, Jesse Victors

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see http://www.gnu.org/licenses/

                For information regarding this software email:
                                Jesse Victors
                         jvictors@jessevictors.com
\******************************************************************************/

#ifndef SLOT_VIEWER
#define SLOT_VIEWER

/*
    SlotViewer handles the viewing of the protein from a particular slot.
    It is given the Trajectory for that slot, and then adds atoms and bonds
    Models to the Scene. The update function animates them by interpolating
    between the available checkpoints. The animation jumps to the first
    checkpoint when it reaches the final one. This is in contrast to FAHViewer,
    which runs the animation backwards.
*/

#include "Trajectory/Trajectory.hpp"
#include "Modeling/Mesh/Mesh.hpp"
#include "Modeling/DataBuffers/ColorBuffer.hpp"
#include "World/Scene.hpp"


typedef std::pair<ProgramPtr, ColorPtr> AtomModelInfo;


template <class T>
inline void hash_combine(std::size_t & seed, const T & v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}



namespace std
{
    template<typename S, typename T> struct hash<pair<S, T>>
    {
        inline size_t operator()(const pair<S, T> & v) const
        {
            size_t seed = 0;
            ::hash_combine(seed, v.first);
            ::hash_combine(seed, v.second);
            return seed;
        }
    };
}



class SlotViewer
{
    public:
        SlotViewer(const TrajectoryPtr& trajectory,
                   const std::shared_ptr<Scene>& scene
        );
        void update(int deltaTime);
        static glm::mat4 alignBetween(const glm::vec3& a, const glm::vec3& b);
        static float getDotProduct(const glm::vec3& vecA, const glm::vec3& vecB);
        static float getMagnitude(const glm::vec3& vector);

    public:
        const float ATOM_SCALE = 0.20f;
        const float BOND_SCALE = 0.10f;

        const unsigned int ATOM_STACKS = 8;
        const unsigned int ATOM_SLICES = 16;

        const glm::vec3 ATOM_LIGHT_POSITION = glm::vec3(-1.5);
        const float ATOM_LIGHT_POWER = 2.0f;
        const glm::vec3 ATOM_LIGHT_COLOR = glm::vec3(4);

        const glm::vec3 BOND_COLOR = glm::vec3(0.8, 0.12, 0.5);
        const float PI = 3.141592653589f;

    private:
        void addAllAtoms();
        void addAllBonds();

        ModelPtr addAtom(const AtomPtr& atom, const glm::mat4& matrix);
        void addBond(const Bond& bond, const ModelPtr& model);

        std::shared_ptr<Mesh> getAtomMesh();
        std::shared_ptr<Mesh> getBondMesh();

        std::shared_ptr<ColorBuffer> generateColorBuffer(const AtomPtr& atom);
        ModelPtr generateAtomModel(const ColorPtr& cBuffer,
                                   const glm::mat4& matrix
        );
        glm::mat4 generateAtomMatrix(const glm::vec3& position);
        glm::mat4 generateBondMatrix(const glm::vec3& startPosition,
                                     const glm::vec3& endPosition
        );

    private:
        TrajectoryPtr trajectory_;
        std::shared_ptr<Scene> scene_;
        std::unordered_map<Bond,    ModelPtr, std::hash<Bond>> bondModels_;
        std::unordered_map<AtomPtr, ModelPtr> atomModels_;

        int transitionTime_; //how much elapsed time between each snapshot
        int snapshotA_, snapshotB_; //used to interpolate between during animation
};

#endif
