<?php

class BldFieldsController {
  //private $BldClient;
  //private $user;


  /**
   * BldFieldsController constructor.
   */
  public function __construct() {
   // $this->user = $user;
    //$this->BldClient = new DrupalBldClient();
  }



 /* public function setNodeGroup($gid, $nid) {
    // Node ID of the group to assign content to.
    $entity = node_load($nid);
    $values = [
      'entity_type' => 'node',
      'entity' => $entity,
    ];
    og_group('node', $gid, $values);
  }
  public function setNodeBody($body, $nid) {
    // Node ID of the group to assign content to.
    $entity = node_load($nid);
    // Body field.
    $entity->body[$entity->language][0]['value'] = $body;
    // If field has a format, you need to define it. Here we define a default filtered_html format for a body field
    $entity->body[$entity->language][0]['format'] = 'full_html';
    node_save($entity);
  }

  public function setNodeImage($image, $nid) {
    // Node ID of the group to assign content to.
    $entity = node_load($nid);
    // Image field.
    if (!is_null($image)) {
      $remoteDocPath = $image;
      //$doc = system_retrieve_file($remoteDocPath, NULL, FALSE, FILE_EXISTS_REPLACE);
      $doc = system_retrieve_file($remoteDocPath, NULL, FALSE, FILE_EXISTS_RENAME);
      $file = $this->bld_add_existing_file($doc);
      $entity->field_image[$entity->language][0] = (array) $file;
      node_save($entity);
    } else {
      drupal_set_message(t('The content field is empty in the service.'),'warning');
    }
  }

  public function setNodeVideo($video, $nid) {
    // Node ID of the group to assign content to.
    $entity = node_load($nid);
    // Video field
    if (!is_null($video['input'])) {
      $entity->field_article_videos[$entity->language][0]['video_url'] = $video['input'];
      node_save($entity);
    } else {
      drupal_set_message(t('The content field is empty in the service.'),'warning');
    }
  }*/

  public function bld_add_existing_file($file_drupal_path, $uid = 1, $status = FILE_STATUS_PERMANENT) {
    $files = file_load_multiple([], ['uri' => $file_drupal_path]);
    $file = reset($files);

    if (!$file) {
      $file = (object) [
        'filename' => basename($file_drupal_path),
        'filepath' => $file_drupal_path,
        'filemime' => file_get_mimetype($file_drupal_path),
        'filesize' => filesize($file_drupal_path),
        'uid' => $uid,
        'status' => $status,
        'timestamp' => time(),
        'uri' => $file_drupal_path,
        'fid' => 'upload',
        'title' => basename($file_drupal_path),
      ];
      drupal_write_record('file_managed', $file);
    }
    return $file;
  }
}

