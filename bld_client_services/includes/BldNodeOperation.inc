<?php

class BldNodeOperation {

  private $language;

  private $user;

  private $title;

  private $type;

  private $status;

  private $promote;

  private $date;

  private $body;

  private $ogGroupRef;

  private $ogGroupLocation;

  private $newsType;

  private $video;

  private $urlImage;

  /**
   * BldNodeOperation constructor.
   */
  public function __construct($language = LANGUAGE_NONE, $user, $title, $type, $status, $promote, $date, $body, $ogGroupRef, $ogGroupLocation, $newsType, $video, $urlImage) {
    $this->language = $language;
    $this->user = $user;
    $this->title = $title;
    $this->type = $type;
    $this->status = $status;
    $this->promote = $promote;
    $this->date = $date;
    $this->body = $body;
    $this->ogGroupRef = $ogGroupRef;
    $this->ogGroupLocation = $ogGroupLocation;
    $this->newsType = $newsType;
    $this->video = $video;
    $this->urlImage = $urlImage;
  }


  public function nodeCreation() {
    $node = new stdClass();
    // Title field.
    $node->title = $this->title;
    // Type field.
    $node->type = $this->type;
    // Sets some defaults. Invokes hook_prepare() and hook_node_prepare().
    node_object_prepare($node);
    // Or e.g. 'en' if locale is enabled.
    //$node->language = LANGUAGE_NONE;
    //Published date
    // Set created date
    //$node->date = $this->date;
    $node->date = date('Y-m-d',strtotime($this->date));
    //$node->date = '2015-01-02 9:30:00';
    //$node->updated = strtotime($this->date);
    $node->created = strtotime($node->date);
    //$node->created = strtotime('2015-01-02 9:30:00');
    // Language field.
    $node->language = $this->language;
    // Uid field.
    $node->uid = $this->user->uid;
    // Status is 1 or 0; published or not.
    // Status field.
    $node->status = $this->status;
    // Promote is 1 or 0; promoted to front page or not.
    // Promote field.
    $node->promote = $this->promote;

    // Body field.
    $node->body[$node->language][0]['value'] = $this->body;
    // If field has a format, you need to define it. Here we define a default filtered_html format for a body field
    $node->body[$node->language][0]['format'] = 'full_html';


    // Entity reference og_group_ref field.
    $node->og_group_ref[$node->language][] = [
      'target_id' => $this->ogGroupRef,
      // Default target_type is 'node', Other possible values are "user",
      // "taxonomy_term" or other valid entity machine name.
      'target_type' => 'node',
    ];

    // Entity reference og_group_location field.
    $node->og_group_location[$node->language][] = [
      //'target_id' => $form_state['values']['entity id'],
      //'target_id' => 13,
      'target_id' => $this->ogGroupLocation,
      // Default target_type is 'node', Other possible values are "user",
      // "taxonomy_term" or other valid entity machine name.
      'target_type' => 'node',
    ];


    switch ($this->newsType) {
      case 'Video':
        // Video field
        if (!is_null($this->video['input'])) {
          $node->field_article_videos[$node->language][0]['video_url'] = $this->video['input'];
        }
        break;
      case 'Image':
        // Image field.
        if (!is_null($this->urlImage)) {
          $remoteDocPath = $this->urlImage;
          $field_controller = new BldFieldsController();
          //$doc = system_retrieve_file($remoteDocPath, NULL, FALSE, FILE_EXISTS_REPLACE);
          $doc = system_retrieve_file($remoteDocPath, NULL, FALSE, FILE_EXISTS_RENAME);
          $file = $field_controller->bld_add_existing_file($doc);
          $node->field_image[$node->language][0] = (array) $file;
          //$node->field_images[$node->language][0] = (array) $file;
        }
        break;
      default:
        break;

    }

    // Prepare node for saving
    $node = node_submit($node);
    return node_save($node);
  }

/*  private function drupal_add_existing_file($file_drupal_path, $uid = 1, $status = FILE_STATUS_PERMANENT) {
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
  }*/

}
