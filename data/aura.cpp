#include "aura.h"

#if ENTITY_MEM_TOOLS
int Aura::allocs = 0;
#endif

/*
AnimationCurve *Aura::getDamageLevelScaling() {
	return this->damageLevelScaling;
}

AnimationCurve *Aura::getAbsorbLevelScaling() {
	return this->absorbLevelScaling;
}

AnimationCurve *Aura::getHealLevelScaling() {
	return this->healLevelScaling;
}*/

/*
void Aura::Start() {
	AbilityScalingDataLoader::getInstance()->GetData(this->abilityScaleDataId)->RegisterOnAssetLoaded(new AbilityScalingDataLoaderHelper::AssetLoadedAction(DELEGATE_FUNC(Aura::OnAuraAbilityScalingDataLoaded, _1)));
	if (this->AuraEffectData->Enabled) {
		this->AuraEffectData->LoadEffect();
	}
}
*/

float Aura::get_damage_scale_for_level(int level) {
	//return this->getDamageLevelScaling()->Evaluate((float)(level));
	return 1;
}
float Aura::get_heal_scale_for_level(int level) {
	//return this->getHealLevelScaling()->Evaluate((float)(level));
	return 1;
}

float Aura::get_absorb_scale_for_level(int level) {
	//return this->getAbsorbLevelScaling()->Evaluate((float)(level));
	return 1;
}

SpellEnums::DiminishingReturnCategory Aura::get_diminishing_category() {
	return _diminishing_category;
}

void Aura::set_diminishing_category(SpellEnums::DiminishingReturnCategory diminishingCategory) {
	_diminishing_category = diminishingCategory;
}

void Aura::set(int id, float time, int auraGroup) {
	this->set_id(id);
	this->set_time(time);
	this->set_aura_group(auraGroup);
}

/*
void Aura::SetScalingData(AbilityScalingData *scalingData) {
	scalingData->getDamageCurve();
	scalingData->getAbsorbCurve();
	scalingData->getHealingCurve();
}*/
/*
void Aura::OnAuraAbilityScalingDataLoaded(AbilityScalingDataLoaderHelper *h) {
	this->SetScalingData(h->getData());
}
*/

void Aura::set_damage(int min, int max, float tick, bool can_crit) {
	set_damage_enabled(true);
	set_damage_min(min);
	set_damage_max(max);
	set_damage_tick(tick);
	set_damage_can_crit(can_crit);
}

void Aura::set_heal(int min, int max, float tick, bool can_crit) {
	set_heal_enabled(true);
	set_heal_min(min);
	set_heal_max(max);
	set_heal_tick(tick);
	set_heal_can_crit(can_crit);
}

Aura::Aura() {
	ability_scale_data_id = 1;
	id = 0;
	time = 0;
	aura_group = 0;

	_damage_enabled = false;
	_damage_type = 0;
	_damage_min = 0;
	_damage_max = 0;
	_damage_tick = 0;
	_damage_can_crit = false;

	_absorb_enabled = false;
	_absorb_damage_type = 0;
	_absorb_min = 0;
	_absorb_max = 0;

	_heal_enabled = false;
	_heal_min = 0;
	_heal_max = 0;
	_heal_tick = 0;
	_heal_can_crit = false;

	_add_states = 0;
	_remove_effects_with_states = 0;
	_supress_states = 0;

	_aura_stat_attribute_count = 0;
	for (int i = 0; i < MAX_AURA_STATS; ++i) {
		_aura_stat_attributes[i] = Ref<AuraStatAttribute>(memnew(AuraStatAttribute()));
	}

	_trigger_count = 0;
	for (int i = 0; i < MAX_TRIGGER_DATA; ++i) {
		_trigger_datas[i] = Ref<AuraTriggerData>(memnew(AuraTriggerData()));
	}

#if ENTITY_MEM_TOOLS
	Aura::allocs++;
	print_error("Aura alloc " + String::num(Aura::allocs));
#endif
}

Aura::~Aura() {
#if ENTITY_MEM_TOOLS
	Aura::allocs--;
	print_error("Aura dealloc " + String::num(Aura::allocs));
#endif
}

//////     Triggers      ///////

int Aura::get_trigger_count() const {
	return _trigger_count;
}

void Aura::set_trigger_count(int count) {
	ERR_FAIL_COND(count < 0 || count > MAX_TRIGGER_DATA);

	_trigger_count = count;
}

SpellEnums::TriggerEvents Aura::get_trigger_event(int index) const {
	ERR_FAIL_INDEX_V(index, _trigger_count, SpellEnums::TRIGGER_NONE);

	return _trigger_datas[index]->get_trigger_event();
}

void Aura::set_trigger_event(int index, const SpellEnums::TriggerEvents value) {
	ERR_FAIL_COND(index < 0 || index > _trigger_count);

	_trigger_datas[index]->set_trigger_event(value);
}

Ref<Aura> Aura::get_trigger_aura(int index) const {
	ERR_FAIL_INDEX_V(index, _trigger_count, Ref<Aura>(NULL));

	return _trigger_datas[index]->get_aura();
}

void Aura::set_trigger_aura(int index, const Ref<Aura> value) {
	ERR_FAIL_COND(index < 0 || index > _trigger_count);

	_trigger_datas[index]->set_aura(value);
}

Ref<Spell> Aura::get_trigger_spell(int index) const {
	ERR_FAIL_INDEX_V(index, _trigger_count, Ref<Spell>(NULL));

	return _trigger_datas[index]->get_spell();
}

void Aura::set_trigger_spell(int index, const Ref<Spell> value) {
	ERR_FAIL_COND(index < 0 || index > _trigger_count);

	_trigger_datas[index]->set_spell(value);
}

////// Aura Stat Attributes //////

int Aura::get_aura_stat_attribute_count() const {
	return _aura_stat_attribute_count;
}
void Aura::set_aura_stat_attribute_count(int count) {
	ERR_FAIL_COND(count < 0 || count > MAX_AURA_STATS);

	_aura_stat_attribute_count = count;
}

Stat::StatId Aura::get_aura_stat_attribute_stat(int index) const {
	ERR_FAIL_INDEX_V(index, MAX_AURA_STATS, Stat::STAT_ID_NONE);

	return _aura_stat_attributes[index]->get_stat();
}
void Aura::set_aura_stat_attribute_stat(int index, const Stat::StatId value) {
	ERR_FAIL_INDEX(index, MAX_AURA_STATS);

	_aura_stat_attributes[index]->set_stat(value);
}

float Aura::get_aura_stat_attribute_base_mod(int index) const {
	ERR_FAIL_INDEX_V(index, MAX_AURA_STATS, 0);

	return _aura_stat_attributes[index]->get_base_mod();
}
void Aura::set_aura_stat_attribute_base_mod(int index, float value) {
	ERR_FAIL_INDEX(index, MAX_AURA_STATS);

	_aura_stat_attributes[index]->set_base_mod(value);
}

float Aura::get_aura_stat_attribute_bonus_mod(int index) const {
	ERR_FAIL_INDEX_V(index, MAX_AURA_STATS, 0);

	return _aura_stat_attributes[index]->get_bonus_mod();
}
void Aura::set_aura_stat_attribute_bonus_mod(int index, float value) {
	ERR_FAIL_INDEX(index, MAX_AURA_STATS);

	_aura_stat_attributes[index]->set_bonus_mod(value);
}

float Aura::get_aura_stat_attribute_percent_mod(int index) const {
	ERR_FAIL_INDEX_V(index, MAX_AURA_STATS, 0);

	return _aura_stat_attributes[index]->get_percent_mod();
}
void Aura::set_aura_stat_attribute_percent_mod(int index, float value) {
	ERR_FAIL_INDEX(index, MAX_AURA_STATS);

	_aura_stat_attributes[index]->set_percent_mod(value);
}

void Aura::sapply_simple(Entity *caster, Entity *target, float spell_scale) {
	Ref<AuraApplyInfo> info(memnew(AuraApplyInfo(caster, target, spell_scale, this)));

	sapply(info);
}


void Aura::sapply(Ref<AuraApplyInfo> info) {
	if (has_method("_sapply")) {
		call_multilevel("_sapply", info);
		return;
	}

	Ref<Aura> aura = info->get_aura();

	Ref<AuraData> ad(memnew(AuraData()));
	setup_aura_data(ad, info);

	info->get_target()->add_aura(ad);
}

void Aura::sremove(Ref<AuraData> aura) {
	aura->get_owner()->sremove_aura(aura);
}

void Aura::sremove_expired(Ref<AuraData> aura) {
	aura->get_owner()->sremove_aura_expired(aura);
}

bool Aura::supdate(Ref<AuraData> aura, float delta) {

	bool remove = aura->update_remaining_time(delta);

	//ontick
	if (aura->get_unhandled_ticks() != 0) {

		if (aura->get_damage() != 0) {
			Ref<SpellDamageInfo> dpd = Ref<SpellDamageInfo>(memnew(SpellDamageInfo()));

			dpd->set_aura_damage_source(aura);
			dpd->set_dealer(aura->get_caster());
			dpd->set_receiver(aura->get_owner());

			handle_aura_damage(aura, dpd);
		}

		aura->set_unhandled_ticks(0);
	}

	if (remove) {
		sremove_expired(aura);
	}

	return remove;
}

void Aura::sdispell(Entity *target, AuraData *data) {
}

void Aura::son_before_cast(Ref<SpellCastInfo> info) {
	if (has_method("_son_before_cast")) {
		call_multilevel("_son_before_cast", info);
		return;
	}
}

void Aura::son_before_cast_target(Ref<SpellCastInfo> info) {
	if (has_method("_son_before_cast_target")) {
		call_multilevel("_son_before_cast_target", info);
		return;
	}
}

void Aura::son_cast_finished(Ref<SpellCastInfo> info) {
	if (has_method("_son_cast_finished")) {
		call_multilevel("_son_cast_finished", info);
		return;
	}
}

void Aura::sapply_passives_damage_receive(Ref<SpellDamageInfo> data) {
	if (has_method("_sapply_passives_damage_receive")) {
		call_multilevel("_sapply_passives_damage_receive", data);
		return;
	}
}

void Aura::sapply_passives_damage_deal(Ref<SpellDamageInfo> data) {
	if (has_method("_sapply_passives_damage_deal")) {
		call_multilevel("_sapply_passives_damage_deal", data);
		return;
	}
}

void Aura::son_before_damage(Ref<SpellDamageInfo> data) {
	if (has_method("_son_before_damage")) {
		call_multilevel("_son_before_damage", data);
		return;
	}
}

void Aura::son_damage_receive(Ref<SpellDamageInfo> data) {
	if (has_method("_son_damage_receive")) {
		call_multilevel("_son_damage_receive", data);
		return;
	}
}

void Aura::son_hit(Ref<SpellDamageInfo> data) {
	if (has_method("_son_hit")) {
		call_multilevel("_son_hit", data);
		return;
	}
}

void Aura::son_damage_dealt(Ref<SpellDamageInfo> data) {
	if (has_method("_son_damage_dealt")) {
		call_multilevel("_son_damage_dealt", data);
		return;
	}
}

void Aura::son_remove_expired(Ref<AuraData> aura) {
	if (has_method("_son_remove_expired")) {
		call_multilevel("_son_remove_expired", aura);
		return;
	}
}

void Aura::son_remove(Ref<AuraData> aura) {
	//Debug::Log(new String("OnRemove called, Deprecated!"));
	//this->DeApply(caster, null);
}

void Aura::son_dispell(Ref<AuraData> aura) {
}

void Aura::son_before_aura_applied(Ref<AuraData> data) {
}

void Aura::son_after_aura_applied(Ref<AuraData> data) {
}

void Aura::con_added(Entity *target, Aura *data, AuraData *aura) {
}

void Aura::con_removed(Entity *target, Aura *data) {
}

void Aura::con_refresh(Entity *target, Object *data, AuraData *aura) {
}

void Aura::setup_aura_data(Ref<AuraData> data, Ref<AuraApplyInfo> info) {
	if (has_method("_setup_aura_data")) {
		call_multilevel("_setup_aura_data", data, info);
		return;
	}

	data->set_aura(Ref<Aura>(this));
	data->set_caster(info->get_caster());

	if (is_damage_enabled()) {
		calculate_initial_damage(data, info);
	}
}

void Aura::calculate_initial_damage(Ref<AuraData> aura_data, Ref<AuraApplyInfo> info) {
	if (has_method("_calculate_initial_damage")) {
		call_multilevel("_calculate_initial_damage", aura_data, info);
		return;
	}

	aura_data->set_damage(info->get_aura()->get_damage_min());
}

void Aura::handle_aura_damage(Ref<AuraData> aura_data, Ref<SpellDamageInfo> data) {
	data->set_damage(aura_data->get_damage());

	if (has_method("_handle_aura_damage")) {
		call_multilevel("_handle_aura_damage", aura_data, data);
		return;
	}

	data->get_dealer()->sdeal_damage_to(data);
}

void Aura::_validate_property(PropertyInfo &property) const {

	String prop = property.name;
	if (prop.begins_with("StatModAttribute_")) {
		int frame = prop.get_slicec('/', 0).get_slicec('_', 1).to_int();
		if (frame >= _aura_stat_attribute_count) {
			property.usage = 0;
		}
	} else if (prop.begins_with("Trigger_")) {
		int frame = prop.get_slicec('/', 0).get_slicec('_', 1).to_int();
		if (frame >= _trigger_count) {
			property.usage = 0;
		}
	}
}



void Aura::_bind_methods() {
	BIND_VMETHOD(MethodInfo("_sapply", PropertyInfo(Variant::OBJECT, "info", PROPERTY_HINT_RESOURCE_TYPE, "AuraApplyInfo")));

	BIND_VMETHOD(MethodInfo("_son_before_cast", PropertyInfo(Variant::OBJECT, "info", PROPERTY_HINT_RESOURCE_TYPE, "SpellCastInfo")));
	BIND_VMETHOD(MethodInfo("_son_before_cast_target", PropertyInfo(Variant::OBJECT, "info", PROPERTY_HINT_RESOURCE_TYPE, "SpellCastInfo")));
	BIND_VMETHOD(MethodInfo("_son_cast_finished", PropertyInfo(Variant::OBJECT, "info", PROPERTY_HINT_RESOURCE_TYPE, "SpellCastInfo")));

	BIND_VMETHOD(MethodInfo("_sapply_passives_damage_receive", PropertyInfo(Variant::OBJECT, "data", PROPERTY_HINT_RESOURCE_TYPE, "SpellDamageInfo")));
	BIND_VMETHOD(MethodInfo("_sapply_passives_damage_deal", PropertyInfo(Variant::OBJECT, "data", PROPERTY_HINT_RESOURCE_TYPE, "SpellDamageInfo")));
	BIND_VMETHOD(MethodInfo("_son_before_damage", PropertyInfo(Variant::OBJECT, "data", PROPERTY_HINT_RESOURCE_TYPE, "SpellDamageInfo")));
	BIND_VMETHOD(MethodInfo("_son_damage_receive", PropertyInfo(Variant::OBJECT, "data", PROPERTY_HINT_RESOURCE_TYPE, "SpellDamageInfo")));
	BIND_VMETHOD(MethodInfo("_son_hit", PropertyInfo(Variant::OBJECT, "data", PROPERTY_HINT_RESOURCE_TYPE, "SpellDamageInfo")));
	BIND_VMETHOD(MethodInfo("_son_damage_dealt", PropertyInfo(Variant::OBJECT, "data", PROPERTY_HINT_RESOURCE_TYPE, "SpellDamageInfo")));

	BIND_VMETHOD(MethodInfo("_setup_aura_data", PropertyInfo(Variant::OBJECT, "data", PROPERTY_HINT_RESOURCE_TYPE, "AuraData"), PropertyInfo(Variant::OBJECT, "data", PROPERTY_HINT_RESOURCE_TYPE, "AuraApplyInfo")));
	BIND_VMETHOD(MethodInfo("_calculate_initial_damage", PropertyInfo(Variant::OBJECT, "data", PROPERTY_HINT_RESOURCE_TYPE, "AuraData") , PropertyInfo(Variant::OBJECT, "data", PROPERTY_HINT_RESOURCE_TYPE, "AuraApplyInfo")));
	BIND_VMETHOD(MethodInfo("_handle_aura_damage", PropertyInfo(Variant::OBJECT, "data", PROPERTY_HINT_RESOURCE_TYPE, "AuraData") , PropertyInfo(Variant::OBJECT, "data", PROPERTY_HINT_RESOURCE_TYPE, "SpellDamageInfo")));

	BIND_VMETHOD(MethodInfo("_son_remove_expired", PropertyInfo(Variant::OBJECT, "data", PROPERTY_HINT_RESOURCE_TYPE, "AuraData")));

	ClassDB::bind_method(D_METHOD("get_id"), &Aura::get_id);
	ClassDB::bind_method(D_METHOD("set_id", "value"), &Aura::set_id);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "id"), "set_id", "get_id");

	ClassDB::bind_method(D_METHOD("get_aura_name"), &Aura::get_aura_name);
	ClassDB::bind_method(D_METHOD("set_aura_name", "value"), &Aura::set_aura_name);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "aura_name"), "set_aura_name", "get_aura_name");

	ClassDB::bind_method(D_METHOD("get_icon"), &Aura::get_icon);
	ClassDB::bind_method(D_METHOD("set_icon", "value"), &Aura::set_icon);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "icon", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_icon", "get_icon");

	ClassDB::bind_method(D_METHOD("get_time"), &Aura::get_time);
	ClassDB::bind_method(D_METHOD("set_time", "value"), &Aura::set_time);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "time"), "set_time", "get_time");

	ClassDB::bind_method(D_METHOD("get_aura_group"), &Aura::get_aura_group);
	ClassDB::bind_method(D_METHOD("set_aura_group", "value"), &Aura::set_aura_group);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "aura_group"), "set_aura_group", "get_aura_group");

	ClassDB::bind_method(D_METHOD("get_ability_scale_data_id"), &Aura::get_ability_scale_data_id);
	ClassDB::bind_method(D_METHOD("set_ability_scale_data_id", "value"), &Aura::set_ability_scale_data_id);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "ability_scale_data_id"), "set_ability_scale_data_id", "get_ability_scale_data_id");

	ADD_GROUP("Damage", "damage");
	//Damage
	ClassDB::bind_method(D_METHOD("is_damage_enabled"), &Aura::is_damage_enabled);
	ClassDB::bind_method(D_METHOD("set_damage_enabled", "value"), &Aura::set_damage_enabled);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "damage_enabled"), "set_damage_enabled", "is_damage_enabled");

	ClassDB::bind_method(D_METHOD("get_damage_type"), &Aura::get_damage_type);
	ClassDB::bind_method(D_METHOD("set_damage_type", "value"), &Aura::set_damage_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "damage_type", PROPERTY_HINT_FLAGS, SpellEnums::BINDING_STRING_DAMAGE_TYPES), "set_damage_type", "get_damage_type");

	ClassDB::bind_method(D_METHOD("get_damage_min"), &Aura::get_damage_min);
	ClassDB::bind_method(D_METHOD("set_damage_min", "value"), &Aura::set_damage_min);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "damage_min"), "set_damage_min", "get_damage_min");

	ClassDB::bind_method(D_METHOD("get_damage_max"), &Aura::get_damage_max);
	ClassDB::bind_method(D_METHOD("set_damage_max", "value"), &Aura::set_damage_max);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "damage_max"), "set_damage_max", "get_damage_max");

	ClassDB::bind_method(D_METHOD("get_damage_tick"), &Aura::get_damage_tick);
	ClassDB::bind_method(D_METHOD("set_damage_tick", "value"), &Aura::set_damage_tick);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "damage_tick"), "set_damage_tick", "get_damage_tick");

	ClassDB::bind_method(D_METHOD("get_damage_can_crit"), &Aura::get_damage_can_crit);
	ClassDB::bind_method(D_METHOD("set_damage_can_crit", "value"), &Aura::set_damage_can_crit);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "damage_can_crit"), "set_damage_can_crit", "get_damage_can_crit");

	ClassDB::bind_method(D_METHOD("set_damage", "min", "max", "tick", "can_crit"), &Aura::set_damage);

	ClassDB::bind_method(D_METHOD("get_damage_scaling_curve"), &Aura::get_damage_scaling_curve);
	ClassDB::bind_method(D_METHOD("set_damage_scaling_curve", "curve"), &Aura::set_damage_scaling_curve);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "damage_scaling_curve", PROPERTY_HINT_RESOURCE_TYPE, "Curve"), "set_damage_scaling_curve", "get_damage_scaling_curve");

	ADD_GROUP("Absorb", "absorb");
	//Absorb
	ClassDB::bind_method(D_METHOD("is_absorb_enabled"), &Aura::is_absorb_enabled);
	ClassDB::bind_method(D_METHOD("set_absorb_enabled", "value"), &Aura::set_absorb_enabled);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "absorb_enabled"), "set_absorb_enabled", "is_absorb_enabled");

	ClassDB::bind_method(D_METHOD("get_absorb_damage_type"), &Aura::get_absorb_damage_type);
	ClassDB::bind_method(D_METHOD("set_absorb_damage_type", "value"), &Aura::set_absorb_damage_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "absorb_damage_type", PROPERTY_HINT_FLAGS, SpellEnums::BINDING_STRING_DAMAGE_TYPES), "set_absorb_damage_type", "get_absorb_damage_type");

	ClassDB::bind_method(D_METHOD("get_absorb_min"), &Aura::get_absorb_min);
	ClassDB::bind_method(D_METHOD("set_absorb_min", "value"), &Aura::set_absorb_min);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "absorb_min"), "set_absorb_min", "get_absorb_min");

	ClassDB::bind_method(D_METHOD("get_absorb_max"), &Aura::get_absorb_max);
	ClassDB::bind_method(D_METHOD("set_absorb_max", "value"), &Aura::set_absorb_max);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "absorb_max"), "set_absorb_max", "get_absorb_max");

	ClassDB::bind_method(D_METHOD("get_absorb_scaling_curve"), &Aura::get_absorb_scaling_curve);
	ClassDB::bind_method(D_METHOD("set_absorb_scaling_curve", "curve"), &Aura::set_absorb_scaling_curve);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "absorb_scaling_curve", PROPERTY_HINT_RESOURCE_TYPE, "Curve"), "set_absorb_scaling_curve", "get_absorb_scaling_curve");

	ADD_GROUP("Heal", "heal");
	//Heal
	ClassDB::bind_method(D_METHOD("is_heal_enabled"), &Aura::is_heal_enabled);
	ClassDB::bind_method(D_METHOD("set_heal_enabled", "value"), &Aura::set_heal_enabled);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "heal_enabled"), "set_heal_enabled", "is_heal_enabled");

	ClassDB::bind_method(D_METHOD("get_heal_min"), &Aura::get_heal_min);
	ClassDB::bind_method(D_METHOD("set_heal_min", "value"), &Aura::set_heal_min);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "heal_min"), "set_heal_min", "get_heal_min");

	ClassDB::bind_method(D_METHOD("get_heal_max"), &Aura::get_heal_max);
	ClassDB::bind_method(D_METHOD("set_heal_max", "value"), &Aura::set_heal_max);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "heal_max"), "set_heal_max", "get_heal_max");

	ClassDB::bind_method(D_METHOD("get_heal_tick"), &Aura::get_heal_tick);
	ClassDB::bind_method(D_METHOD("set_heal_tick", "value"), &Aura::set_heal_tick);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "heal_tick"), "set_heal_tick", "get_heal_tick");

	ClassDB::bind_method(D_METHOD("get_heal_can_crit"), &Aura::get_heal_can_crit);
	ClassDB::bind_method(D_METHOD("set_heal_can_crit", "value"), &Aura::set_heal_can_crit);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "heal_can_crit"), "set_heal_can_crit", "get_heal_can_crit");

	ClassDB::bind_method(D_METHOD("set_heal", "min", "max", "tick", "can_crit"), &Aura::set_heal);

	ClassDB::bind_method(D_METHOD("get_heal_scaling_curve"), &Aura::get_heal_scaling_curve);
	ClassDB::bind_method(D_METHOD("set_heal_scaling_curve", "curve"), &Aura::set_heal_scaling_curve);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "heal_scaling_curve", PROPERTY_HINT_RESOURCE_TYPE, "Curve"), "set_heal_scaling_curve", "get_heal_scaling_curve");

	//ADD_GROUP("States", "states"); not needed
	ClassDB::bind_method(D_METHOD("get_add_states"), &Aura::get_add_states);
	ClassDB::bind_method(D_METHOD("set_add_states", "value"), &Aura::set_add_states);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "states/add_states", PROPERTY_HINT_FLAGS, EntityEnums::BINDING_STRING_ENTITY_STATE_TYPES), "set_add_states", "get_add_states");

	ClassDB::bind_method(D_METHOD("get_remove_effects_with_states"), &Aura::get_remove_effects_with_states);
	ClassDB::bind_method(D_METHOD("set_remove_effects_with_states", "value"), &Aura::set_remove_effects_with_states);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "states/remove_effects_with_states", PROPERTY_HINT_FLAGS, EntityEnums::BINDING_STRING_ENTITY_STATE_TYPES), "set_remove_effects_with_states", "get_remove_effects_with_states");

	ClassDB::bind_method(D_METHOD("get_supress_states"), &Aura::get_supress_states);
	ClassDB::bind_method(D_METHOD("set_supress_states", "value"), &Aura::set_supress_states);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "states/supress_states", PROPERTY_HINT_FLAGS, EntityEnums::BINDING_STRING_ENTITY_STATE_TYPES), "set_supress_states", "get_supress_states");

	////    Triggers    ////
	ADD_GROUP("Triggers", "trigger");
	ClassDB::bind_method(D_METHOD("get_trigger_count"), &Aura::get_trigger_count);
	ClassDB::bind_method(D_METHOD("set_trigger_count", "count"), &Aura::set_trigger_count);

	ClassDB::bind_method(D_METHOD("get_trigger_event", "index"), &Aura::get_trigger_event);
	ClassDB::bind_method(D_METHOD("set_trigger_event", "index", "value"), &Aura::set_trigger_event);

	ClassDB::bind_method(D_METHOD("get_trigger_aura", "index"), &Aura::get_trigger_aura);
	ClassDB::bind_method(D_METHOD("set_trigger_aura", "index", "value"), &Aura::set_trigger_aura);

	ClassDB::bind_method(D_METHOD("get_trigger_spell", "index"), &Aura::get_trigger_spell);
	ClassDB::bind_method(D_METHOD("set_trigger_spell", "index", "value"), &Aura::set_trigger_spell);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "trigger_count", PROPERTY_HINT_RANGE, "0," + itos(MAX_TRIGGER_DATA), PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_UPDATE_ALL_IF_MODIFIED), "set_trigger_count", "get_trigger_count");

	for (int i = 0; i < MAX_TRIGGER_DATA; i++) {
		ADD_PROPERTYI(PropertyInfo(Variant::INT, "Trigger_" + itos(i) + "/trigger_event", PROPERTY_HINT_ENUM, SpellEnums::BINDING_STRING_TRIGGER_EVENTS, PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_INTERNAL), "set_trigger_event", "get_trigger_event", i);
		ADD_PROPERTYI(PropertyInfo(Variant::OBJECT, "Trigger_" + itos(i) + "/trigger_aura", PROPERTY_HINT_RESOURCE_TYPE, "Aura", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_INTERNAL), "set_trigger_aura", "get_trigger_aura", i);
		ADD_PROPERTYI(PropertyInfo(Variant::OBJECT, "Trigger_" + itos(i) + "/trigger_spell", PROPERTY_HINT_RESOURCE_TYPE, "Spell", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_INTERNAL), "set_trigger_spell", "get_trigger_spell", i);
	}

	ADD_GROUP("Attributes", "attribute");
	//AuraStatAttributes
	ClassDB::bind_method(D_METHOD("get_aura_stat_attribute_count"), &Aura::get_aura_stat_attribute_count);
	ClassDB::bind_method(D_METHOD("set_aura_stat_attribute_count", "count"), &Aura::set_aura_stat_attribute_count);

	ClassDB::bind_method(D_METHOD("get_aura_stat_attribute_stat", "index"), &Aura::get_aura_stat_attribute_stat);
	ClassDB::bind_method(D_METHOD("set_aura_stat_attribute_stat", "index", "value"), &Aura::set_aura_stat_attribute_stat);

	ClassDB::bind_method(D_METHOD("get_aura_stat_attribute_base_mod", "index"), &Aura::get_aura_stat_attribute_base_mod);
	ClassDB::bind_method(D_METHOD("set_aura_stat_attribute_base_mod", "index", "value"), &Aura::set_aura_stat_attribute_base_mod);

	ClassDB::bind_method(D_METHOD("get_aura_stat_attribute_bonus_mod", "index"), &Aura::get_aura_stat_attribute_bonus_mod);
	ClassDB::bind_method(D_METHOD("set_aura_stat_attribute_bonus_mod", "index", "value"), &Aura::set_aura_stat_attribute_bonus_mod);

	ClassDB::bind_method(D_METHOD("get_aura_stat_attribute_percent_mod", "index"), &Aura::get_aura_stat_attribute_percent_mod);
	ClassDB::bind_method(D_METHOD("set_aura_stat_attribute_percent_mod", "index", "value"), &Aura::set_aura_stat_attribute_percent_mod);

	ClassDB::bind_method(D_METHOD("sapply", "info"), &Aura::sapply);
	ClassDB::bind_method(D_METHOD("get_aura_stat_attribute", "index"), &Aura::get_aura_stat_attribute);
	//ClassDB::bind_method(D_METHOD("sremove", "info"), &Aura::sremove);
	//ClassDB::bind_method(D_METHOD("supdate", "info"), &Aura::supdate);
	//ClassDB::bind_method(D_METHOD("sdispell", "info"), &Aura::sdispell);


	ADD_PROPERTY(PropertyInfo(Variant::INT, "attribute_count", PROPERTY_HINT_RANGE, "0," + itos(MAX_AURA_STATS), PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_UPDATE_ALL_IF_MODIFIED), "set_aura_stat_attribute_count", "get_aura_stat_attribute_count");

	for (int i = 0; i < MAX_AURA_STATS; i++) {
		ADD_PROPERTYI(PropertyInfo(Variant::INT, "StatModAttribute_" + itos(i) + "/stat", PROPERTY_HINT_ENUM, Stat::STAT_BINDING_STRING, PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_INTERNAL), "set_aura_stat_attribute_stat", "get_aura_stat_attribute_stat", i);
		ADD_PROPERTYI(PropertyInfo(Variant::REAL, "StatModAttribute_" + itos(i) + "/base_mod", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_INTERNAL), "set_aura_stat_attribute_base_mod", "get_aura_stat_attribute_base_mod", i);
		ADD_PROPERTYI(PropertyInfo(Variant::REAL, "StatModAttribute_" + itos(i) + "/bonus_mod", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_INTERNAL), "set_aura_stat_attribute_bonus_mod", "get_aura_stat_attribute_bonus_mod", i);
		ADD_PROPERTYI(PropertyInfo(Variant::REAL, "StatModAttribute_" + itos(i) + "/percent_mod", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_INTERNAL), "set_aura_stat_attribute_percent_mod", "get_aura_stat_attribute_percent_mod", i);
	}


	BIND_CONSTANT(MAX_AURA_STATS);
	BIND_CONSTANT(MAX_TRIGGER_DATA);

	//ClassDB::bind_method(D_METHOD("get_damage_scale_for_level"), &Aura::get_damage_scale_for_level);
	//ClassDB::bind_method(D_METHOD("get_heal_scale_for_level"), &Aura::get_heal_scale_for_level);
	//ClassDB::bind_method(D_METHOD("get_absorb_scale_for_level"), &Aura::get_absorb_scale_for_level);
}



/*

void AuraScript::RemoveAura(Entity* caster)
{
	AuraComponent* auraComponent = caster->AuraComponent;
	AuraData* data = null;
	for (int i = 0; i < auraComponent->serverAuras->Count; i += 1) {
		if (auraComponent->serverAuras->GetData(i)->AuraId == this->auraId) {
			data = auraComponent->serverAuras->GetData(i);
			break;
		}
	}
	this->DeApply(caster, data);
	auraComponent->JustRemoveAura(this->auraId);
}

bool AuraScript::ShouldApplyModifiers(Entity* target)
{
	AuraComponent* auraComponent = target->AuraComponent;
	for (int i = 0; i < auraComponent->serverAuras->Count; i += 1) {
		if (auraComponent->serverAuras->GetData(i)->AuraId == this->auraId) {
			return false;
		}
	}
	return true;
}

bool AuraScript::BasicAuraUpdate(Entity* target, AuraData* data)
{
	data->Remaining -= Time::deltaTime;
	if (data->Remaining <= (float)0) {
		data->Remaining = (float)0;
		this->DeApply(target, data);
		return true;
	}
	return false;
}

bool AuraScript::ShouldApplyModifiers(Entity* target, float refreshTo)
{
	AuraComponent* auraComponent = target->AuraComponent;
	for (int i = 0; i < auraComponent->serverAuras->Count; i += 1) {
		if (auraComponent->serverAuras->GetData(i)->AuraId == this->auraId) {
			auraComponent->RefreshAura(this->auraId, refreshTo);
			return false;
		}
	}
	return true;
}

bool AuraScript::ShouldApplyModifiers(Entity* target, float refreshTo, Entity* caster, float spellScale)
{
	AuraComponent* auraComponent = target->AuraComponent;
	for (int i = 0; i < auraComponent->serverAuras->Count; i += 1) {
		if (auraComponent->serverAuras->GetData(i)->AuraId == this->auraId) {
			auraComponent->RefreshAura(this->auraId, refreshTo);
			return false;
		}
	}
	this->AddAuraDataToTarget(target, caster, refreshTo, spellScale);
	return true;
}

bool AuraScript::ShouldRemoveModifiers(Entity* target)
{
	AuraComponent* auraComponent = target->AuraComponent;
	int num = 0;
	for (int i = 0; i < auraComponent->serverAuras->Count; i += 1) {
		if (auraComponent->serverAuras->GetData(i)->AuraId == this->auraId) {
			num += 1;
			if (num > 1) {
				return false;
			}
		}
	}
	return true;
}

void AuraScript::RefreshDamageCountAuraDiminished(Entity* target, float refreshTo, Entity* caster, DiminishingReturnAuraData::DiminishingReturnCategory diminsihingType, DamageCountAuraData* aura)
{
	int auraIdForDiminishingReturn = this->GetAuraIdForDiminishingReturn(diminsihingType);
	if (auraIdForDiminishingReturn == -1) {
		Debug::Log(*(new String("AuraScript:RefreshDamageCountAuraDiminished-> Auraid is -1 for a diminishing return aura! DiminishingType: ")) + diminsihingType->ToString());
		return;
	}
	AuraComponent* auraComponent = target->AuraComponent;
	AuraData* auraData = this->getAuraData(target, auraIdForDiminishingReturn);
	DiminishingReturnAuraData* aura2 = null;
	if (auraData != null) {
		if (!is_inst_of<DiminishingReturnAuraData*>(auraData)) {
			Debug::Log(*(new String("AuraScript:RefreshDamageCountAuraDiminished-> Auradata isn't DiminishingReturnAuraData! auraId:")) + BOX<int>(auraIdForDiminishingReturn));
			return;
		}
		auraComponent->RefreshAura(auraData->AuraId, (float)15);
	}
	else {
		this->ApplyAura(auraIdForDiminishingReturn, caster, target, (float)1);
		auraData = this->getAuraData(target, auraIdForDiminishingReturn);
		if (!is_inst_of<DiminishingReturnAuraData*>(auraData)) {
			Debug::Log(*(new String("AuraScript:RefreshDamageCountAuraDiminished-> Auradata isn't DiminishingReturnAuraData! auraId:")) + BOX<int>(auraIdForDiminishingReturn));
			return;
		}
		aura2 = (DiminishingReturnAuraData*)(auraData);
	}
	float diminishedTime = this->GetDiminishedTime(target, aura2, refreshTo);
	auraComponent->RefreshAura(auraData->AuraId, diminishedTime);
}

void AuraScript::AddAuraDataToTarget(Entity* target, AuraData* data)
{
	target->AuraComponent->AddAura(data);
}

void AuraScript::AddAuraDataToTarget(Entity* target, Entity* caster, float duration, float spellScale)
{
	BaseAuraComponent* arg_2D_0 = target->AuraComponent;
	AuraData* data = new AuraData(this->auraId, duration, caster, caster->PlayerData->GUID, this->aura->AuraGroup, spellScale);
	arg_2D_0->AddAura(data);
}

void AuraScript::AddOrRefreshDamageAuraDataToTarget(Entity* target, Entity* caster, float spellScale, float duration, float tick, int damage)
{
	AuraData* auraData = this->TargetHasCastersAura(target, caster);
	if (auraData == null) {
		DamageAuraData* data = new DamageAuraData(this->auraId, duration, caster, caster->PlayerData->GUID, this->aura->AuraGroup, spellScale, tick, damage);
		this->AddAuraDataToTarget(target, data);
		return;
	}
	BaseAuraComponent* arg_5F_0 = target->AuraComponent;
	DamageAuraData* damageAuraData = (DamageAuraData*)(*auraData);
	arg_5F_0->RefreshAura(damageAuraData->AuraId, (Entity*)(damageAuraData->Caster), duration);
	damageAuraData->Damage = damage;
}

void AuraScript::AddOrRefreshHealAuraDataToTarget(Entity* target, Entity* caster, float spellScale, float duration, float tick, int heal)
{
	AuraData* auraData = this->TargetHasCastersAura(target, caster);
	if (auraData == null) {
		HealAuraData* data = new HealAuraData(this->auraId, duration, caster, caster->PlayerData->GUID, this->aura->AuraGroup, spellScale, tick, heal);
		this->AddAuraDataToTarget(target, data);
		return;
	}
	BaseAuraComponent* arg_5F_0 = target->AuraComponent;
	HealAuraData* healAuraData = (HealAuraData*)(*auraData);
	arg_5F_0->RefreshAura(healAuraData->AuraId, (Entity*)(healAuraData->Caster), duration);
	healAuraData->Heal = heal;
}

void AuraScript::AddOrRefreshShieldAuraDataToTarget(Entity* target, Entity* caster, float spellScale, float duration, int absorb)
{
	AuraData* auraData = this->TargetHasAura(target);
	if (auraData == null) {
		ShieldAuraData* data = new ShieldAuraData(this->auraId, duration, caster, caster->PlayerData->GUID, this->aura->AuraGroup, spellScale, absorb);
		this->AddAuraDataToTarget(target, data);
		return;
	}
	BaseAuraComponent* arg_5C_0 = target->AuraComponent;
	ShieldAuraData* shieldAuraData = (ShieldAuraData*)(*auraData);
	arg_5C_0->RefreshAura(shieldAuraData->AuraId, (Entity*)(shieldAuraData->Caster), duration);
	shieldAuraData->RemainingAbsorb = absorb;
}

void AuraScript::AddOrRefreshDiminishingReturnAura(Entity* target, DiminishingReturnAuraData::DiminishingReturnCategory diminishingCategory)
{
	int auraIdForDiminishingReturn = this->GetAuraIdForDiminishingReturn(diminishingCategory);
	if (auraIdForDiminishingReturn == -1) {
		Debug::Log(*(new String("AuraScript:AddOrRefreshDiminishingReturnAura-> Auraid is -1 for a diminishing return aura! DiminishingType: ")) + diminishingCategory->ToString());
		return;
	}
	AuraData* auraData = this->getAuraData(target, auraIdForDiminishingReturn);
	if (auraData == null) {
		this->ApplyAura(auraIdForDiminishingReturn, target, target, (float)1);
		return;
	}
	if (!is_inst_of<DiminishingReturnAuraData*>(auraData)) {
		Debug::Log(*(new String("AuraScript:AddOrRefreshDiminishingReturnAura-> Auradata isn't DiminishingReturnAuraData! auraId:")) + BOX<int>(auraIdForDiminishingReturn));
		return;
	}
	DiminishingReturnAuraData* diminishingReturnAuraData = (DiminishingReturnAuraData*)(*auraData);
	target->AuraComponent->RefreshAura(diminishingReturnAuraData->AuraId, (float)15);
}

void AuraScript::AddStatModifier(Entity* target, int stat, float maxMod, float percentMod)
{
	target->Stats->GetSStatFromId(stat)->Modifiers->Add(this->auraId, maxMod, percentMod, true);
}

void AuraScript::RemoveStatModifier(Entity* target, int stat)
{
	target->Stats->GetSStatFromId(stat)->Modifiers->Remove(this->auraId, true);
}

void AuraScript::AddState(Entity* target, StateData::StateType state)
{
	target->StateComponent->AddState(this->auraId, state);
}

void AuraScript::RemoveState(Entity* target, StateData::StateType state)
{
	target->StateComponent->RemoveStateWithAuraId(this->auraId, state);
}

void AuraScript::RemovethisAura(Entity* target)
{
	BaseAuraComponent* arg_14_0 = target->AuraComponent;
	this->DeApply(target, null);
	arg_14_0->JustRemoveAura(this->auraId);
}

AuraData* AuraScript::TargetHasCastersAura(Entity* target, Entity* caster)
{
	AuraComponent* auraComponent = target->AuraComponent;
	for (int i = 0; i < auraComponent->serverAuras->Count; i += 1) {
		if ((auraComponent->serverAuras->GetData(i)->AuraId == this->auraId) && (auraComponent->serverAuras->GetData(i)->Caster == *caster)) {
			return auraComponent->serverAuras->GetData(i);
		}
	}
	return null;
}

AuraData* AuraScript::TargetHasAura(Entity* target)
{
	AuraComponent* auraComponent = target->AuraComponent;
	for (int i = 0; i < auraComponent->serverAuras->Count; i += 1) {
		if (auraComponent->serverAuras->GetData(i)->AuraId == this->auraId) {
			return auraComponent->serverAuras->GetData(i);
		}
	}
	return null;
}

int AuraScript::CalculateDamage(Entity* caster, Entity* target, float spellScale)
{
	AuraDamageData* auraDamageData = this->aura->AuraDamageData;
	float arg_24_0 = (float)(UnityEngine::Random::Range(auraDamageData->Min, auraDamageData->Max));
	PlayerData* playerData = caster->PlayerData;
	float arg_4D_0 = (float)((int)((arg_24_0 * this->aura->GetDamageScaleForLevel(playerData->Level)) * spellScale));
	Stats* expr_40 = caster->Stats;
	PercentStat* sMeleeDamage = expr_40->SMeleeDamage;
	PercentStat* sDamageTaken = expr_40->SDamageTaken;
	return (int)((arg_4D_0 * (sMeleeDamage->ModPercent / (float)100)) * (sDamageTaken->ModPercent / (float)100));
}

int AuraScript::CalculateHeal(Entity* caster, Entity* target, float spellScale)
{
	AuraHealData* auraHealData = this->aura->AuraHealData;
	float arg_24_0 = (float)(UnityEngine::Random::Range(auraHealData->Min, auraHealData->Max));
	PlayerData* playerData = caster->PlayerData;
	float arg_46_0 = (float)((int)((arg_24_0 * this->aura->GetHealScaleForLevel(playerData->Level)) * spellScale));
	PercentStat* sSpellDamage = caster->Stats->SSpellDamage;
	return (int)(arg_46_0 * (sSpellDamage->ModPercent / (float)100));
}

int AuraScript::CalculateAbsorb(Entity* caster, Entity* target, float spellScale)
{
	AuraAbsorbData* auraAbsorbData = this->aura->AuraAbsorbData;
	float arg_24_0 = (float)(UnityEngine::Random::Range(auraAbsorbData->min, auraAbsorbData->max));
	PlayerData* playerData = caster->PlayerData;
	float arg_46_0 = (float)((int)((arg_24_0 * this->aura->GetAbsorbScaleForLevel(playerData->Level)) * spellScale));
	PercentStat* sSpellDamage = caster->Stats->SSpellDamage;
	return (int)(arg_46_0 * (sSpellDamage->ModPercent / (float)100));
}

void AuraScript::UpdateDamageCountAura(Entity* target, AuraData* aura, int damage, float breakHealthPercent)
{
	DamageCountAuraData* expr_06 = (DamageCountAuraData*)(*aura);
	expr_06->DamageTaken += damage;
	Stats* stats = target->Stats;
	if ((float)(expr_06->DamageTaken) >= (stats->SHealth->Max * (float)0.3)) {
		this->RemovethisAura(target);
	}
}

bool AuraScript::UpdateDamageAura(Entity* target, AuraData* data)
{
	DamageAuraData* damageAuraData = (DamageAuraData*)(*data);
	DamageAuraData* expr_08 = damageAuraData;
	expr_08->TimeSinceLastTick += Time::deltaTime;
	if (damageAuraData->TimeSinceLastTick >= damageAuraData->Tick) {
		DamageAuraData* expr_28 = damageAuraData;
		expr_28->TimeSinceLastTick -= damageAuraData->Tick;
		this->DealDamage(target, damageAuraData);
	}
	return this->BasicAuraUpdate(target, data);
}

bool AuraScript::UpdateDamageAuraTickBool(Entity* target, AuraData* data)
{
	DamageAuraData* damageAuraData = (DamageAuraData*)(*data);
	DamageAuraData* expr_08 = damageAuraData;
	expr_08->TimeSinceLastTick += Time::deltaTime;
	if (damageAuraData->TimeSinceLastTick >= damageAuraData->Tick) {
		DamageAuraData* expr_28 = damageAuraData;
		expr_28->TimeSinceLastTick -= damageAuraData->Tick;
		this->DealDamage(target, damageAuraData);
		return true;
	}
	return false;
}

bool AuraScript::UpdateHealAura(Entity* target, AuraData* data)
{
	HealAuraData* healAuraData = (HealAuraData*)(*data);
	HealAuraData* expr_08 = healAuraData;
	expr_08->TimeSinceLastTick += Time::deltaTime;
	if (healAuraData->TimeSinceLastTick >= healAuraData->Tick) {
		HealAuraData* expr_28 = healAuraData;
		expr_28->TimeSinceLastTick -= healAuraData->Tick;
		this->Heal(target, healAuraData);
	}
	return this->BasicAuraUpdate(target, data);
}

void AuraScript::AbsorbPOnBeforeDamage(SpellDamageInfo* data, AuraData* aura)
{
	ShieldAuraData* shieldAuraData = (ShieldAuraData*)(*aura);
	int num = data->Damage;
	if (shieldAuraData->RemainingAbsorb >= num) {
		ShieldAuraData* expr_18 = shieldAuraData;
		expr_18->RemainingAbsorb -= num;
		num = 0;
	}
	else {
		num -= *shieldAuraData->RemainingAbsorb;
		shieldAuraData->RemainingAbsorb = 0;
	}
	data->Damage = num;
	if (shieldAuraData->RemainingAbsorb == 0) {
		BaseAuraComponent* arg_70_0 = (Entity*)(data->Receiver)->AuraComponent;
		this->DeApply((Entity*)(data->Receiver), aura);
		arg_70_0->JustRemoveAura(this->auraId);
	}
}

void AuraScript::DealDamage(Entity* target, DamageAuraData* data)
{
	this->damagePipelineData->Set(data->Damage, false, SpellType::TYPE_MAGIC, data->Caster, target, this->aura->gameObject);
	AuraComponent* auraComponent = target->AuraComponent;
	auraComponent->OnHit((Entity*)(data->Caster), target, this->aura, this->damagePipelineData->Damage);
	auraComponent->POnBeforeDamage(this->damagePipelineData);
	if (this->damagePipelineData->Damage == 0) {
		return;
	}
	target->Stats->TakeDamage(this->damagePipelineData->Damage, false, (Entity*)(data->Caster));
	auraComponent->OnDamageReceive((Entity*)(data->Caster), target, this->aura, this->damagePipelineData->Damage);
}

void AuraScript::DealDamage(Entity* target, DamageAuraData* data, int damage)
{
	this->damagePipelineData->Set(damage, false, SpellType::TYPE_MAGIC, data->Caster, target, this->aura->gameObject);
	AuraComponent* auraComponent = target->AuraComponent;
	auraComponent->OnHit((Entity*)(data->Caster), target, this->aura, this->damagePipelineData->Damage);
	auraComponent->POnBeforeDamage(this->damagePipelineData);
	if (this->damagePipelineData->Damage == 0) {
		return;
	}
	target->Stats->TakeDamage(this->damagePipelineData->Damage, false, (Entity*)(data->Caster));
	auraComponent->OnDamageReceive((Entity*)(data->Caster), target, this->aura, this->damagePipelineData->Damage);
}

void AuraScript::DealDamage(Entity* target, Entity* caster, int damage)
{
	this->damagePipelineData->Set(damage, false, SpellType::TYPE_MAGIC, caster, target, this->aura->gameObject);
	AuraComponent* auraComponent = target->AuraComponent;
	auraComponent->OnHit(caster, target, this->aura, this->damagePipelineData->Damage);
	auraComponent->POnBeforeDamage(this->damagePipelineData);
	if (this->damagePipelineData->Damage == 0) {
		return;
	}
	target->Stats->TakeDamage(this->damagePipelineData->Damage, false, caster);
	auraComponent->OnDamageReceive(caster, target, this->aura, this->damagePipelineData->Damage);
}

void AuraScript::DealDamageWithoutOnHit(Entity* target, Entity* caster, int damage)
{
	this->damagePipelineData->Set(damage, false, SpellType::TYPE_MAGIC, caster, target, this->aura->gameObject);
	AuraComponent* auraComponent = target->AuraComponent;
	auraComponent->POnBeforeDamage(this->damagePipelineData);
	if (this->damagePipelineData->Damage == 0) {
		return;
	}
	target->Stats->TakeDamage(this->damagePipelineData->Damage, false, caster);
	auraComponent->OnDamageReceive(caster, target, this->aura, this->damagePipelineData->Damage);
}

void AuraScript::Heal(Entity* target, HealAuraData* data)
{
	target->Stats->TakeHeal(data->Heal, false, (Entity*)(data->Caster));
}

void AuraScript::Heal(Entity* target, HealAuraData* data, int heal)
{
	target->Stats->TakeHeal(heal, false, (Entity*)(data->Caster));
}

void AuraScript::Heal(Entity* target, AuraData* data, int heal)
{
	target->Stats->TakeHeal(heal, false, (Entity*)(data->Caster));
}

DiminishingReturnAuraData* AuraScript::GetDiminishingReturnAuraDataFor(Entity* target, DiminishingReturnAuraData::DiminishingReturnCategory type)
{
	AuraComponent* auraComponent = target->AuraComponent;
	for (int i = 0; i < auraComponent->serverAuras->Count; i += 1) {
		AuraData* auraData = auraComponent->serverAuras->GetData(i);
		if (is_inst_of<DiminishingReturnAuraData*>(auraData)) {
			DiminishingReturnAuraData* diminishingReturnAuraData = (DiminishingReturnAuraData*)(*auraData);
			if (diminishingReturnAuraData->DiminishingCategory == DiminishingReturnAuraData::DiminishingReturnCategory::TYPE_ROOT) {
				return diminishingReturnAuraData;
			}
		}
	}
	return null;
}

float AuraScript::GetDiminishedTime(Entity* target, DiminishingReturnAuraData::DiminishingReturnCategory diminsihingType, float time)
{
	int auraIdForDiminishingReturn = this->GetAuraIdForDiminishingReturn(diminsihingType);
	if (auraIdForDiminishingReturn == -1) {
		Debug::Log(*(new String("AuraScript:RefreshDamageCountAuraDiminished-> Auraid is -1 for a diminishing return aura! DiminishingType: ")) + diminsihingType->ToString());
		return (float)0;
	}
	AuraData* auraData = this->getAuraData(target, auraIdForDiminishingReturn);
	if (auraData != null) {
		DiminishingReturnAuraData* aura = (DiminishingReturnAuraData*)(*auraData);
		return this->GetDiminishedTime(target, aura, time);
	}
	return time;
}

void AuraScript::AddEffect(Entity* player, EffectPoints bodyPart, Quaternion* rotation)
{
	Transform* effectPoint = player->GetComponent<IEffectPointGetter>()->GetEffectPoint(bodyPart);
	if (effectPoint == null) {
		return;
	}
	GameObject* expr_37 = UnityEngine::Object::Instantiate<GameObject>(this->aura->AuraEffectData->EffectPrefab, Vector3::zero, Quaternion::identity, effectPoint);
	expr_37->AddComponent<EffectAuraIdAttribute>()->AuraId = this->auraId;
	expr_37->transform->localRotation = rotation;
	expr_37->transform->localPosition = Vector3::zero;
}

void AuraScript::AddEffect(Entity* player, GameObject* effectPrefab, EffectPoints bodyPart, Quaternion* rotation)
{
	Transform* effectPoint = player->GetComponent<IEffectPointGetter>()->GetEffectPoint(bodyPart);
	if (effectPoint == null) {
		return;
	}
	GameObject* expr_28 = UnityEngine::Object::Instantiate<GameObject>(effectPrefab, Vector3::zero, Quaternion::identity, effectPoint);
	expr_28->AddComponent<EffectAuraIdAttribute>()->AuraId = this->auraId;
	expr_28->transform->localRotation = rotation;
	expr_28->transform->localPosition = Vector3::zero;
}

void AuraScript::RemoveEffect(Entity* player, EffectPoints bodyPart)
{
	Transform* effectPoint = player->GetComponent<IEffectPointGetter>()->GetEffectPoint(bodyPart);
	if (effectPoint == null) {
		return;
	}
	for (int i = 0; i < effectPoint->childCount; i += 1) {
		Transform* child = effectPoint->GetChild(i);
		EffectAuraIdAttribute* component = child->GetComponent<EffectAuraIdAttribute>();
		if (!component == null && (component->AuraId == this->auraId)) {
			child->SetParent(null);
			UnityEngine::Object::Destroy(child->gameObject);
			return;
		}
	}
}

void AuraScript::RemoveActiveGroupAuras(Entity* target)
{
	if (this->aura->AuraGroup == 0) {
		return;
	}
	target->AuraComponent->RemoveAurasWithGroup(this->aura->AuraGroup);
}

float AuraScript::GetDiminishedTime(Entity* target, DiminishingReturnAuraData* aura, float time)
{
	if (aura != null) {
		if (aura->Count <= 4) {
			time /= (float)(aura->Count);
		}
		else {
			time = (float)0;
		}
	}
	return time;
}

int AuraScript::GetAuraIdForDiminishingReturn(DiminishingReturnAuraData::DiminishingReturnCategory type)
{
	if (type == DiminishingReturnAuraData::DiminishingReturnCategory::TYPE_ROOT) {
		return 1;
	}
	return -1;
}

AuraData* AuraScript::getAuraData(Entity* target, int pAuraId)
{
	AuraComponent* auraComponent = target->AuraComponent;
	for (int i = 0; i < auraComponent->serverAuras->Count; i += 1) {
		if (auraComponent->serverAuras->GetData(i)->AuraId == pAuraId) {
			return auraComponent->serverAuras->GetData(i);
		}
	}
	return null;
}

void AuraScript::ApplyAura(int auraId, Entity* caster, Entity* target, float spellScale)
{
	(AuraScript*)(Auras::Instance->GetData(auraId)->AuraScript)->Apply(caster, target, spellScale);
}

bool AuraScript::IsZero(float x)
{
	return (x < Mathf::Epsilon) && (x > -Mathf::Epsilon);
}


*/


		/*
GenericAuraScript::GenericAuraScript() {
    if (aura->GenericAuraScriptData->AttributeModifiers->Count > 0) {
        this->hasModifiers = true;
    }
}
void GenericAuraScript::Apply(WorldEntity* caster, WorldEntity* target, float spellScale)
{
    if (this->aura->AuraGroup != 0) {
        AuraScript::RemoveActiveGroupAuras(target);
    }
    if (((this->hasModifiers && !this->aura->AuraDamageData->Enabled) && !this->aura->AuraAbsorbData->Enabled) && !this->aura->AuraHealData->Enabled) {
        if (AuraScript::ShouldApplyModifiers(target, this->aura->Time, caster, spellScale)) {
            this->ApplyModifiers(target);
        }
        return;
    }
    if (this->aura->AuraDamageData->Enabled) {
        AuraScript::AddOrRefreshDamageAuraDataToTarget(target, caster, spellScale, this->aura->Time, this->aura->AuraDamageData->Tick, AuraScript::CalculateDamage(caster, target, spellScale));
    }
    if (this->aura->AuraHealData->Enabled) {
        AuraScript::AddOrRefreshHealAuraDataToTarget(target, caster, spellScale, this->aura->Time, this->aura->AuraHealData->Tick, AuraScript::CalculateHeal(caster, target, spellScale));
    }
    if (this->aura->AuraAbsorbData->Enabled) {
        AuraScript::AddOrRefreshShieldAuraDataToTarget(target, caster, spellScale, this->aura->Time, AuraScript::CalculateAbsorb(caster, target, spellScale));
    }
}
void GenericAuraScript::DeApply(WorldEntity* target, AuraData* data)
{
    if (this->hasModifiers && AuraScript::ShouldRemoveModifiers(target)) {
        this->RemoveModifiers(target);
    }
}
bool GenericAuraScript::AuraUpdate(WorldEntity* target, AuraData* data)
{
    bool flag = false;
    if (this->aura->AuraDamageData->Enabled) {
        flag = AuraScript::UpdateDamageAura(target, data);
    }
    if (this->aura->AuraHealData->Enabled) {
        bool flag2 = AuraScript::UpdateHealAura(target, data);
        if (!flag) {
            flag = flag2;
        }
    }
    if (!this->aura->AuraDamageData->Enabled && !this->aura->AuraHealData->Enabled) {
        return AuraScript::AuraUpdate(target, data);
    }
    return flag;
}
void GenericAuraScript::POnBeforeDamage(SpellDamageInfo* data, AuraData* auraData)
{
    if (this->aura->AuraAbsorbData->Enabled) {
        AuraScript::AbsorbPOnBeforeDamage(data, auraData);
    }
}
void GenericAuraScript::COnAdded(WorldEntity* target, Aura* data, AuraData* auraData)
{
    this->ApplyEffect(target);
}
void GenericAuraScript::COnRemoved(WorldEntity* target, Aura* data)
{
    this->RemoveEffect(target);
}
void GenericAuraScript::ApplyEffect(WorldEntity* target)
{
    if (!this->aura->AuraEffectData->Enabled) {
        return;
    }
    if (this->aura->AuraEffectData->EffectPoint1 != EffectPoints::NONE) {
        AuraScript::AddEffect(target, this->aura->AuraEffectData->EffectPrefab, this->aura->AuraEffectData->EffectPoint1, );
    }
    if (this->aura->AuraEffectData->EffectPoint2 != EffectPoints::NONE) {
        AuraScript::AddEffect(target, this->aura->AuraEffectData->EffectPrefab, this->aura->AuraEffectData->EffectPoint2, );
    }
}
void GenericAuraScript::RemoveEffect(WorldEntity* target)
{
    if (!this->aura->AuraEffectData->Enabled) {
        return;
    }
    if (this->aura->AuraEffectData->EffectPoint1 != EffectPoints::NONE) {
        AuraScript::RemoveEffect(target, this->aura->AuraEffectData->EffectPoint1);
    }
    if (this->aura->AuraEffectData->EffectPoint2 != EffectPoints::NONE) {
        AuraScript::RemoveEffect(target, this->aura->AuraEffectData->EffectPoint2);
    }
}
void GenericAuraScript::ApplyModifiers(WorldEntity* target)
{
    for (int i = 0; i < this->aura->GenericAuraScriptData->AttributeModifiers->Count; i += 1) {
        GenericAuraScriptAttributeHelper* genericAuraAttributeHelper = this->aura->GenericAuraScriptData->AttributeModifiers->GetData(i);
        AuraScript::AddStatModifier(target, genericAuraAttributeHelper->Stat, genericAuraAttributeHelper->MaxMod, genericAuraAttributeHelper->PercentMod);
    }
}
void GenericAuraScript::RemoveModifiers(WorldEntity* target)
{
    for (int i = 0; i < this->aura->GenericAuraScriptData->AttributeModifiers->Count; i += 1) {
        GenericAuraScriptAttributeHelper* genericAuraAttributeHelper = this->aura->GenericAuraScriptData->AttributeModifiers->GetData(i);
        AuraScript::RemoveStatModifier(target, genericAuraAttributeHelper->Stat);
    }
}
*/




