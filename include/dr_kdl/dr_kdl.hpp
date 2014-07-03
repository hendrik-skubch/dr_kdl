#pragma once
#include <map>
#include <string>

#include <sensor_msgs/JointState.h>
#include <kdl/tree.hpp>
#include <Eigen/Geometry>

namespace dr {


/// Get a the transform from the base to the end of a chain.
/**
 * Throws if a non-fixed joint is encountered in the chain.
 * \return The transform from the base of the chain to the end.
 */
Eigen::Isometry3d getTransform(
	KDL::Chain const & chain ///< The chain.
);


/// Get a the transform from the base to the end of a chain.
/**
 * Non-fixed joints are looked up in a map.
 * Throws if a joint is not found.
 * \return The transform from the base of the chain to the end.
 */
Eigen::Isometry3d getTransform(
	KDL::Chain const & chain,                    ///< The chain.
	std::map<std::string, double> const & joints ///< The map of joint positions to use for non fixed joints in the chain.
);


/// Get a the transform from the base to the end of a chain.
/**
 * Non-fixed joints are looked up in a pair of vectors.
 * Throws if a joint is not found.
 * \return The transform from the base of the chain to the end.
 */
Eigen::Isometry3d getTransform(
	KDL::Chain const & chain,                     ///< The chain.
	std::vector<std::string> const & joint_names, ///< The names of the joints in same order as the joint position vector.
	std::vector<double> const & joint_positions   ///< The positions of the joints in the same order as the joint name vector.
);


/// KDL tree wrapper.
class KdlTree : public KDL::Tree {
public:
	KdlTree() {}
	KdlTree(KDL::Tree const &  tree) : KDL::Tree{tree} {}
	KdlTree(KDL::Tree const && tree) : KDL::Tree{std::move(tree)} {}

	static KdlTree fromString(std::string const & urdf);
	static KdlTree fromParameter(std::string const & parameter);
	static KdlTree fromFile(std::string const & filename);

	/// Get a KDL chain between two segments.
	KDL::Chain getChain(
		std::string const & start, /// The start segment.
		std::string const & end    /// The end segment.
	) const;

	/// Get a transform from one frame to another.
	/**
	 * Throws if there is no chain between the frames or the chain contains a non-fixed joint.
	 */
	Eigen::Isometry3d transform(
		std::string const & source, ///< The source frame.
		std::string const & target  ///< The target frame.
	) const {
		return getTransform(getChain(source, target));
	}

	/// Get a transform from one frame to another.
	/**
	 * Throws if there is no chain between the frames or the chain contains a non-fixed joint for which no joint position is given.
	 */
	Eigen::Isometry3d transform(
		std::string const & source,                   ///< The source frame.
		std::string const & target,                   ///< The target frame.
		std::map<std::string, double> const & joints  ///< The map holding joint positions.
	) const {
		return getTransform(getChain(source, target), joints);
	}

	/// Get a transform from one frame to another.
	/**
	 * Throws if there is no chain between the frames or the chain contains a non-fixed joint for which no joint position is given.
	 */
	Eigen::Isometry3d transform(
		std::string const & source,                   ///< The source frame.
		std::string const & target,                   ///< The target frame.
		std::vector<std::string> const & joint_names, ///< The names of the joints in same order as the joint position vector.
		std::vector<double> const & joint_positions   ///< The positions of the joints in the same order as the joint name vector.
	) const {
		return getTransform(getChain(source, target), joint_names, joint_positions);
	}

	/// Get a transform from one frame to another.
	/**
	 * Throws if there is no chain between the frames or the chain contains a non-fixed joint for which no joint position is given.
	 */
	Eigen::Isometry3d transform(
		std::string const & source,                   ///< The source frame.
		std::string const & target,                   ///< The target frame.
		sensor_msgs::JointState const & joints        ///< The joint positions.
	) const {
		return getTransform(getChain(source, target), joints.name, joints.position);
	}


};

}