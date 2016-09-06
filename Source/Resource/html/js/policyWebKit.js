var policyTreeAjax = (function() {
    var getData = function() {
        /**
         * Get the json for jstree
         *
         * @return json
         * {"policiesTree":POLICIES_JSTREE_JSON}
         */
        res = webpage.get_policies_tree();
        data = JSON.parse(res);
        if (!data.error)
            policyTree.setData(data.policiesTree);
        else
            mcoMessage.error(data.error);
    };

    var policyImport = function(form) {
        /**
         * Import a policy from an XML (the XML is provided as POST data from a form)
         *
         * Call policy_import and policy_get
         *
         * @return json
         * {"policy":POLICY_JSTREE_JSON}
         */
        res = webpage.import_policy();
        data = JSON.parse(res);
        $("#xslPolicyImport_policyFile").val("");
        if (!data.error)
            policyTree.policyImport(data.policyTree);
        else
            mcoMessage.error(data.error);
    }

    var policyCreate = function(policyNode, parentId) {
        /**
         * Create a policy
         * @param int parentId policy ID in which the new policy will be created
         *
         * Call xslt_policy_create and policy_get
         *
         * @return json
         * {"policy":POLICY_JSTREE_JSON}
         */
        res = webpage.xslt_policy_create(parentId);
        data = JSON.parse(res);
        if (!data.error)
            policyTree.policyCreate(data.policyTree, policyNode);
        else
            mcoMessage.error(data.error);
    }

    var policyEdit = function(form, policyNode) {
        /**
         * Edit a policy (POST data from a form)
         * @param int id policy ID of the policy to edit
         *
         * @return json
         * {"policy":POLICY_JSTREE_JSON}
         */
        var name = $("#xslPolicyInfo_policyName").val();
        if (name === null)
            name = "";

        var description = $("#xslPolicyInfo_policyDescription").val();
        if (description === null)
            description = "";

        var type = $("#xslPolicyInfo_policyType").val();
        if (type === null)
            type = "";

        res = webpage.policy_edit(policyNode.data.policyId, name, description, type);
        data = JSON.parse(res);
        if (!data.error)
            policyTree.policyEdit(data.policyTree, policyNode);
        else
            mcoMessage.error(data.error);
    }

    var policyDelete = function(policyNode) {
        /**
         * Delete a policy
         * @param int id policy ID of the policy to duplicate
         *
         * @return json
         * {"policyId":ID}
         */
        res = webpage.policy_remove(policyNode.data.policyId);
        data = JSON.parse(res);
        if (!data.error)
            policyTree.policyDelete(policyNode);
        else
            mcoMessage.error(data.error);
    }

    var policyExport = function(policyNode) {
        /**
        * Export XML of a policy
        * @param int id policy ID of the policy to export
        *
        * @return XML
        */
        res = webpage.policy_export(policyNode.data.policyId);
        data = JSON.parse(res);
        if (data.error)
            mcoMessage.error(data.error);
    }

    var policyDuplicate = function(policyNode, dstNode) {
        /**
         * Duplicate a policy
         * @param int id policy ID of the policy to duplicate
         * @param int dstPolicyId policy ID of the destination policy
         *
         * @return json
         * {"policy":POLICY_JSTREE_JSON}
         */
        res = webpage.policy_duplicate(policyNode.data.policyId, policyTree.getPolicyId(dstNode));
        data = JSON.parse(res);
        if (!data.error)
            policyTree.policyDuplicate(data.policyTree, dstNode);
        else
            mcoMessage.error(data.error);
    }

    var policyMove = function(policyNode, dstNode) {
        /**
         * Move a policy
         * @param int id policy ID of the policy to duplicate
         * @param int dstPolicyId policy ID of the destination policy
         *
         * @return json
         * {"policy":POLICY_JSTREE_JSON}
         */
        res = webpage.policy_move(policyNode.data.policyId, policyTree.getPolicyId(dstNode));
        data = JSON.parse(res);
        if (!data.error)
            policyTree.policyMove(data.policyTree, dstNode, policyNode);
        else
            mcoMessage.error(data.error);
    }

    var ruleCreate = function(policyNode) {
        /**
         * Add a rule to a policy
         * @param int policyId policy ID of the policy that will contain the rule
         *
         * @return json
         * {"rule":{"tracktype":TRACKTYPE, "field":FIELD, "id":RULE_ID, "name":NAME, "value":VALUE, "occurrence":OCCURENCE,  "ope":OPERATOR}}
         */
        res = webpage.xslt_policy_rule_create(policyNode.data.policyId);
        data = JSON.parse(res);
        if (!data.error)
            policyTree.ruleCreate(data.rule, policyNode);
        else
            mcoMessage.error(data.error);
    }

    var ruleEdit = function(form, policyId, ruleNode) {
        /**
         * Edit a rule (POST data from a form)
         * @param int id rule ID of the rule to edit
         * @param int policyId policy ID of the policy that contain the rule
         *
         * @return json
         * {"rule":{"tracktype":TRACKTYPE, "field":FIELD, "id":RULE_ID, "name":NAME, "value":VALUE, "occurrence":OCCURENCE, "ope":OPERATOR}}
         */

        var name = $("#xslPolicyRule_title").val();
        if (name === null)
            name = "";

        var tracktype = $("#xslPolicyRule_trackType").val();
        if (tracktype === null)
            tracktype = "";

        var field = $("#xslPolicyRule_field").val();
        if (field === null)
            field = "";

        var occurrence = $("#xslPolicyRule_occurrence").val();
        if (occurrence === null || occurrence === "*")
            occurrence = -1;

        var ope = $("#xslPolicyRule_validator").val();
        if (ope === null)
            ope = "";

        var value = $("#xslPolicyRule_value").val();
        if (value === null)
            value = "";

        res = webpage.xslt_policy_rule_edit(ruleNode.data.ruleId, policyId, name, tracktype, field, occurrence, ope, value);
        data = JSON.parse(res);
        if (!data.error)
            policyTree.ruleEdit(data.rule, ruleNode);
        else
            mcoMessage.error(data.error);
    }

    var ruleDelete = function(policyId, ruleNode) {
        /**
         * Delete a rule
         * @param int id rule ID of the rule to delete
         * @param int policyId policy ID of the policy that contain the rule
         *
         * @return json
         * {"id":RULE_ID}
         */
        res = webpage.xslt_policy_rule_delete(policyId, ruleNode.data.ruleId);
        data = JSON.parse(res);
        if (!data.error)
            policyTree.ruleDelete(ruleNode);
        else
            mcoMessage.error(data.error);
    }

    var ruleDuplicate = function(policyId, ruleNode, dstNode) {
        /**
         * Duplicate a rule
         * @param int id rule ID of the rule to duplicate
         * @param int policyId policy ID of the policy that contain the rule
         * @param int dstPolicyId policy ID of the destination policy
         *
         * @return json
         * {"rule":{"tracktype":TRACKTYPE, "field":FIELD, "id":RULE_ID, "name":NAME, "value":VALUE, "occurrence":OCCURENCE,  "ope":OPERATOR}}
         */

        res = webpage.xslt_policy_rule_duplicate(policyId, ruleNode.data.ruleId, dstNode.data.policyId);
        data = JSON.parse(res);
        if (!data.error)
            policyTree.ruleDuplicate(data.rule, dstNode);
        else
            mcoMessage.error(data.error);
    }

    var ruleMove = function(policyId, ruleNode, dstNode) {
        /**
         * Move a rule
         * @param int id rule ID of the rule to move
         * @param int policyId policy ID of the policy that contain the rule
         * @param int dstPolicyId policy ID of the destination policy
         *
         * @return json
         * {"rule":{"tracktype":TRACKTYPE, "field":FIELD, "id":RULE_ID, "name":NAME, "value":VALUE, "occurrence":OCCURENCE, "ope":OPERATOR}}
         */

        res = webpage.xslt_policy_rule_move(policyId, ruleNode.data.ruleId, dstNode.data.policyId);
        data = JSON.parse(res);
        if (!data.error)
            policyTree.ruleMove(data.rule, dstNode, ruleNode);
        else
            mcoMessage.error(data.error);
    }

    var getFieldsList = function(trackType, field) {
        /**
         * Get list of fields for a trackType (POST : type and field)
         *
         * @return json
         */
        res = webpage.get_fields_list(trackType, field);
        data = JSON.parse(res);
        if (!data.error)
            policyTreeRules.fieldsListOk(data.fields, field)
        else
            policyTreeRules.fieldsListError(field);
    }

    var getValuesList = function(trackType, field, value) {
        /**
         * Get list of values for a trackType and a field (POST : type, field and value)
         *
         * @return json
         */
        res = webpage.get_values_list(trackType, field, value);
        data = JSON.parse(res);
        if (!data.error)
            policyTreeRules.valuesListOk(data.values, value);
        else
            policyTreeRules.valuesListError(value);
    }

    return {
        getData: getData,
        policyImport: policyImport,
        policyCreate: policyCreate,
        policyEdit: policyEdit,
        policyDelete: policyDelete,
        policyDuplicate: policyDuplicate,
        policyMove: policyMove,
        policyExport: policyExport,
        ruleCreate: ruleCreate,
        ruleEdit: ruleEdit,
        ruleDelete: ruleDelete,
        ruleDuplicate: ruleDuplicate,
        ruleMove: ruleMove,
        getFieldsList: getFieldsList,
        getValuesList: getValuesList,
    };
})();
