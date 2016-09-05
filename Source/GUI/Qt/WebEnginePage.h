/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a GPLv3+/MPLv2+ license that can
 *  be found in the License.html file in the root of the source tree.
 */

#ifndef WEBENGINEPAGE_H
#define WEBENGINEPAGE_H

#include <QMap>
#include <QWebEnginePage>
#include <QWebEngineView>
#include "mainwindow.h"

namespace MediaConch {

class FileRegistered;
class XsltPolicy;
class XsltPolicyRule;

class WebPage : public QWebEnginePage
{
    Q_OBJECT

public:
    explicit WebPage(MainWindow *m, QWidget *parent = 0);

    void         change_local_files(const QStringList& files);
    void         use_javascript(const QString& js);

protected:
    virtual bool acceptNavigationRequest(const QUrl& url, QWebEnginePage::NavigationType type, bool isMainFrame);
    QStringList  chooseFiles(FileSelectionMode mode, const QStringList &oldFiles, const QStringList &acceptedMimeTypes);

    void         clean_forms();
    void         add_sub_directory_files_to_list(const QDir dir, QFileInfoList& list);
    void         charge_local_dir(const QString& directory, QStringList& tmp);
    QStringList  choose_file_settings();
    QStringList  choose_file_import_policy();
    void         string_to_json(QString& str);

public Q_SLOTS:
    void         on_file_upload_selected(const QString& policy, const QString& display_xslt, const QString& verbosity);
    void         on_file_online_selected(const QString& url, const QString& policy, const QString& display_xslt, const QString& verbosity);
    void         on_file_repository_selected(const QString& policy, const QString& display_xslt, const QString& verbosity);
    void         on_save_settings_selected(const QString& policy, const QString& display, const QString& verbosity,
                                           const QString& save_report, const QString& load_files,
                                           const QString& save_policy, const QString& load_policy,
                                           const QString& save_display, const QString& load_display);

    void         on_input_changed(const QString& inputName);
    void         menu_link_checker(const QString& name);

    void         on_download_report(const QString& report, const QString& file, const QString& report_name);
    QString      on_fill_implementation_report(const QString& name, const QString& display, const QString& verbosity);
    void         on_save_implementation_report(const QString& file, const QString& display = "-1", const QString& verbosity="");
    QString      on_fill_policy_report(const QString& file, const QString& policy, const QString& display);
    void         on_save_policy_report(const QString& file, const QString& policy = "-1", const QString& display = "-1");
    QString      on_fill_mediainfo_report(const QString& file);
    void         on_save_mediainfo_report(const QString& file);
    QString      on_fill_mediatrace_report(const QString& file);
    void         on_save_mediatrace_report(const QString& file);
    QString      on_create_policy_from_file(const QString& file);

    void         close_all();
    void         close_element(const QString& file);

    bool         report_is_html(const QString& report);
    bool         report_is_xml(const QString& report);

    QString      get_file_tool(const QString& file);
    QString      get_file_policy_id(const QString& file);
    QString      get_file_display_id(const QString& file);
    QString      get_file_verbosity_id(const QString& file);
    bool         policy_is_valid(const QString& file);
    bool         implementation_is_valid(const QString& file);
    QString      file_is_analyzed(const QString& file);
    QString      change_policy_for_file(const QString& file, const QString& policy, const QString& fileId);


    QString      display_add_file(const QString& name);
    void         display_export_id(const QString& name);
    void         display_delete_id(const QString& name);

    QString      get_policies_tree();
    void         create_rule_tree(XsltPolicyRule *r, int index, QString& rule_data);
    QString      import_policy();
    QString      import_policy(const QString& file);
    int          import_policy(const QStringList& files);
    QString      xslt_policy_create(int parent_id);
    QString      duplicate_policy(int id, int dst_policy_id);
    QString      move_policy(int id, int dst_policy_id);
    QString      export_policy(int id);
    QString      policy_remove(int id);
    QString      policy_change_info(int id, const QString& name, const QString& description);
    QString      policy_change_type(int id, const QString& type);
    QString      policy_rule_create(int policy_id);
    QString      policy_rule_edit(int policy_id, int rule_id, const QString& title, bool is_editor, const QString& type, const QString& field, int occurrence, const QString& ope, const QString& value, const QString& free_text);
    QString      policy_rule_duplicate(int policy_id, int rule_id, int dst_policy_id);
    QString      policy_rule_move(int policy_id, int rule_id, int dst_policy_id);
    QString      policy_rule_delete(int policy_id, int rule_id);
    QString      get_values_list(const QString& type, const QString& field, const QString& value);
    QString      get_fields_list(const QString& type, const QString& field);

private Q_SLOTS:
    void         on_load_finished(bool ok);

protected:
    MainWindow                 *mainwindow;
    QMap<QString, QStringList>  file_selector;
    QString                     select_file_name;
};

}

#endif
