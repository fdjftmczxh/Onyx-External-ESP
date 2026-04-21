// 作者: feng_xingzhe_cn_.86

#pragma once
#ifndef ONYX_SDK_GAME_SKELETON_HPP
#define ONYX_SDK_GAME_SKELETON_HPP

#include <onyx/sdk/engine/Transform.hpp>
#include <onyx/sdk/engine/structures/Quaternion.hpp>
#include <onyx/sdk/Offsets.hpp>
#include <vector>
#include <optional>
#include <android/log.h>

namespace onyx::sdk::game {

/// @brief 骨骼关节数据。
struct BoneJoint {

    sdk::Vector3 world_position;

    int32_t      depth;
    int32_t      parent_index;

};

/// @brief 骨骼系统包装器。
/// 使用 Eager Loading 模式：在构造时遍历并缓存所有骨骼坐标。
struct Skeleton {

    engine::Transform      root_transform;
    sdk::Vector3           root_pos;
    Quaternion             root_rot;

    std::vector<BoneJoint> joints;

    explicit Skeleton(engine::Transform transform_root, sdk::Vector3 world_pos, Quaternion world_rot) : root_transform(transform_root), root_pos(world_pos), root_rot(world_rot) {

        if (root_transform.native_ptr) {

            joints.reserve(80);
            
            sdk::Vector3 root_local = root_transform.local_position();
            Quaternion root_local_rot = root_transform.local_rotation();

            sdk::Vector3 root_world = root_rot * root_local;

            root_world.x += root_pos.x;
            root_world.y += root_pos.y;
            root_world.z += root_pos.z;

            Quaternion root_world_rot = root_rot * root_local_rot;

            collect_recursive(root_transform, 0, -1, root_world, root_world_rot);

        }
    }

    /// @brief 从角色 Transform 自动导航到骨骼子树。
    [[nodiscard]] static std::optional<Skeleton> find_from_character(const engine::Transform& char_root, sdk::Vector3 char_world_pos) noexcept {

        if (!char_root.native_ptr) return std::nullopt;

        engine::Transform armature(static_cast<uintptr_t>(0));

        int32_t max_children = 0;

        for (int32_t i = 0; i < char_root.child_count(); ++i) {

            auto child = char_root.get_child(i);

            int32_t cc = child.child_count();

            if (cc > max_children) {

                max_children = cc;
                armature = child;

            }

        }

        if (!armature.native_ptr) return std::nullopt;

        Quaternion world_rot = char_root.local_rotation() * armature.local_rotation();
        sdk::Vector3 world_pos = char_root.local_rotation() * armature.local_position();

        world_pos.x += char_world_pos.x;
        world_pos.y += char_world_pos.y;
        world_pos.z += char_world_pos.z;

        for (int32_t i = 0; i < armature.child_count(); ++i) {

            auto container = armature.get_child(i);

            if (container.child_count() > 0) {

                auto bone_root = container.get_child(0);

                if (bone_root.native_ptr && bone_root.child_count() >= 3) {

                    Quaternion final_rot = world_rot * container.local_rotation();
                    sdk::Vector3 final_pos = world_rot * container.local_position();

                    final_pos.x += world_pos.x;
                    final_pos.y += world_pos.y;
                    final_pos.z += world_pos.z;

                    return Skeleton(bone_root, final_pos, final_rot);

                }

            }

        }

        return std::nullopt;

    }

private:

    void collect_recursive(const engine::Transform& node, int32_t depth, int32_t parent_idx, const sdk::Vector3& world_pos, const Quaternion& world_rot) noexcept {

        BoneJoint joint;

        joint.world_position = world_pos;
        joint.depth = depth;
        joint.parent_index = parent_idx;

        int32_t my_index = static_cast<int32_t>(joints.size());

        joints.push_back(joint);

        int32_t count = node.child_count();

        if (count <= 0 || count > 128) return;

        for (int32_t i = 0; i < count; ++i) {

            auto child = node.get_child(i);

            if (!child.native_ptr) continue;

            sdk::Vector3 child_local = child.local_position();
            Quaternion child_local_rot = child.local_rotation();

            sdk::Vector3 child_world = world_rot * child_local;

            child_world.x += world_pos.x;
            child_world.y += world_pos.y;
            child_world.z += world_pos.z;

            Quaternion child_world_rot = world_rot * child_local_rot;

            collect_recursive(child, depth + 1, my_index, child_world, child_world_rot);

        }

    }

};

} // namespace onyx::sdk::game

#endif // ONYX_SDK_GAME_SKELETON_HPP
